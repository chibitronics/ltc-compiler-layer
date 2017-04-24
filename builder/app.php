<?php

require_once 'System.php';

class CompilerV2Handler
{
    private $builder_prefs_raw;
    private $builder_prefs;

    /**
     * \brief Processes a compile request.
     *
     * \param string $request The body of the POST request.
     * \return A message to be JSON-encoded and sent back to the requestor.
     */
    function main($request, $config)
    {
        $log = [];

        // The tmp folder where logfiles and object files are placed
        $temporaryDirectory = $config['temp_dir'];

        // The directory name where archive files are stored in $temporaryDirectory
        $ARCHIVE_DIR = $config['archive_dir'];

        $start_time = microtime(true);

        // Step 0: Reject the request if the input data is not valid.
        $err = $this->validateInput($request);
        if ($err != null)
            return $this->makeError("Invalid input: $err");

        $this->setVariables($request, $libraries, $should_archive, $config);

        $incoming_files = array();

        // Step 1(part 1): Extract the project files included in the request.
        $ret = $this->extractFiles($request['files'], $temporaryDirectory, $project_dir, $incoming_files, 'files');
        if ($ret != null)
            return $this->makeError($ret, 1);

        // Add the compiler temp directory to the config struct.
        $config['project_dir'] = $project_dir;

        // Where compiled files go
        $config['output_dir'] = "$project_dir/output";

        // Where the compiler and base libraries live
        $config['base_dir'] = $config['arduino_cores_dir'] . '/v' . $config['version'];

        // This is used, for example, to provide object files, and to provide output files.
        $config['project_name'] = str_replace($config['project_dir'] . '/files/',
                '',
                $incoming_files['ino'][0]) . '.ino';

        // Set up a default library dir, but set it to empty so it won't be used by default.
        $config['lib_dir'] = '';

        // Step 1(part 2): Extract the library files included in the request.
        $files['libs'] = [];
        foreach ($libraries as $library => $library_files) {
            $lib_dir = $config['lib_dir'];
            $ret = $this->extractFiles($library_files, $temporaryDirectory, $lib_dir,
                $files['libs'][$library], $library, true);
            if ($ret != null)
                return $this->makeError($ret, 2);
            $config['lib_dir'] = $lib_dir;
        }

        $ARCHIVE_PATH = '';
        if ($should_archive) {
            $archiveCreated = $this->createArchive($project_dir, $temporaryDirectory, $ARCHIVE_DIR, $ARCHIVE_PATH);
            if ($archiveCreated['success'] !== true) {
                return $archiveCreated;
            }
        }

        //Set logging to true if requested, and create the directory where logfiles are stored.
        $ret = $this->setLoggingParams($request, $config, $temporaryDirectory, $project_dir);
        if ($ret != null)
            return array_merge($this->makeError($ret, 2),
                            ($should_archive) ? array("archive" => $ARCHIVE_PATH) : array());

        // Log the names of the project files and the libraries used in it.
        $this->makeLogEntry($request, $config, $should_archive, $ARCHIVE_PATH);

        // Step 4: Syntax-check and compile source files.
        $arduinoBuilderResult = $this->handleCompile("$project_dir/files", $incoming_files, $config);
        if (array_key_exists('builder_time', $arduinoBuilderResult)) {
            $config['builder_time'] = $arduinoBuilderResult['builder_time'];
        }

        // Step 4.5: Save the cache for future builds
        $this->saveCache($config);

        if ($config['logging'] === true && $arduinoBuilderResult['log']) {
            foreach ($arduinoBuilderResult['log'] as $line) {
                array_push($log, $line);
            }
        }

        if ($should_archive) {
            $archiveCreated = $this->createArchive($project_dir, $temporaryDirectory, $ARCHIVE_DIR, $ARCHIVE_PATH);
            $arduinoBuilderResult['archive'] = $ARCHIVE_PATH;
            if ($archiveCreated['success'] !== true) {
                $arduinoBuilderResult['archive'] = $archiveCreated['message'];
            }
        }

        if ($arduinoBuilderResult['success'] !== true) {
            return $arduinoBuilderResult;
        }

        // Step 8: Convert the output to hex and measure its size.
        $convertedOutput = $this->convertOutput($start_time, $config);

        if ($config['logging'] === true) {
            $convertedOutput['log'] =$log;
        }

        if ($should_archive) {
            $archiveCreated = $this->createArchive($project_dir, $temporaryDirectory, $ARCHIVE_DIR, $ARCHIVE_PATH);
            $convertedOutput['archive'] = $ARCHIVE_PATH;
            if ($archiveCreated['success'] !== true) {
                $convertedOutput['archive'] = $archiveCreated['message'];
            }
        }
        return $convertedOutput;
    }

    private function makeError($msg, $step = 0)
    {
        return array(
            "success" => false,
            "step" => $step,
            "message" => $msg);
    }

    private function copyRecursive($src, $dst)
    {

        if (is_file($src)) {
            if (file_exists($dst) && is_dir($dst))
                return "Destination exists already, and is a directory.";

            if (!copy($src, $dst))
                return "Unable to copy $src to $dst.";
            return null;
        }

        if (!is_dir($dst))
            if (!mkdir($dst, 0775, true))
                return "Unable to create directory $dst.";

        // The target directory exists.  Copy all files over.
        $dirent = dir($src);
        if (!$dirent)
            return "Unable to open directory " . $src . " for copying files.";

        while (false !== ($filename = $dirent->read())) {
            if (($filename == '.') || ($filename == '..'))
                continue;

            $ret = $this->copyRecursive($src . "/" . $filename, $dst . "/" . $filename);
            if ($ret != null) {
                $dirent->close();
                return $ret;
            }
        }
        $dirent->close();

        return null;
    }

    private function copyCaches($sourceDirectory, $destinationDirectory, $caches)
    {
        if (!file_exists($sourceDirectory))
            return null;

        // Ensure the target core directory exists
        if (!file_exists($destinationDirectory))
            if (!mkdir($destinationDirectory, 0777, true))
                return "Unable to create output dir";

        // Go through each of the cache types and copy them, if they exist
        foreach ($caches as $dir) {
            if (!file_exists($sourceDirectory . "/" . $dir))
                continue;

            $ret = $this->copyRecursive($sourceDirectory . "/" . $dir, $destinationDirectory . "/" . $dir);
            if ($ret != null)
                return $ret;
        }

        return null;
    }

    private function updateAccessTimesRecursive($dir, $pattern)
    {
        // The target directory exists.  Copy all files over.
        if (!file_exists($dir))
            return array(
                "success" => true,
                "message" => "Cache directory " . $dir . " does not exist.");

        if (!is_dir($dir))
            return array(
                "success" => false,
                "message" => "Cache directory " . $dir . " is not a directory.");

        $dirent = dir($dir);
        if (!$dirent)
            return array(
                "success" => false,
                "message" => "Unable to open directory " . $dir . " for updating access times.");

        while (false !== ($filename = $dirent->read())) {
            if (($filename == '.') || ($filename == '..'))
                continue;

            if ((substr($filename, strlen($filename) - strlen($pattern)) === $pattern)
                && file_exists($dir . "/" . $filename)
            ) {
                $ret = touch($dir . "/" . $filename);
                if (!$ret) {
                    $dirent->close();
                    return array(
                        "success" => false,
                        "message" => "Unable to update " . $dir . "/" . $filename . " access time.");
                }
            }

            // Recurse into subdirectories, if we've encountered a subdir.
            if (is_dir($dir . "/" . $filename)) {
                $ret = $this->updateAccessTimesRecursive($dir . "/" . $filename, $pattern);
                if ($ret["success"] != true) {
                    $dirent->close();
                    return $ret;
                }
            }
        }
        $dirent->close();

        return array("success" => true);
    }

    private function updateDependencyPathsRecursive($dir, $old_dir, $new_dir)
    {
        $pattern = ".d";

        // The target directory exists.  Copy all files over.
        if (!file_exists($dir))
            return array(
                "success" => true,
                "message" => "Cache directory " . $dir . " does not exist.");

        if (!is_dir($dir))
            return array(
                "success" => false,
                "message" => "Cache directory " . $dir . " is not a directory.");

        $dirent = dir($dir);
        if (!$dirent)
            return array(
                "success" => false,
                "message" => "Unable to open directory " . $dir . " for updating access times.");

        while (false !== ($filename = $dirent->read())) {
            if (($filename == '.') || ($filename == '..'))
                continue;

            if ((substr($filename, strlen($filename) - strlen($pattern)) === $pattern)
                && file_exists($dir . "/" . $filename)
            ) {
                $ret = touch($dir . "/" . $filename);
                $content = file_get_contents($dir . "/" . $filename);
                $ret = file_put_contents($dir . "/" . $filename, str_replace($old_dir, $new_dir, $content));
                if (!$ret) {
                    $dirent->close();
                    return array(
                        "success" => false,
                        "message" => "Unable to update " . $dir . "/" . $filename . " paths.");
                }
            }

            // Recurse into subdirectories, if we've encountered a subdir.
            if (is_dir($dir . "/" . $filename)) {
                $ret = $this->updateDependencyPathsRecursive($dir . "/" . $filename, $old_dir, $new_dir);
                if ($ret["success"] != true) {
                    $dirent->close();
                    return $ret;
                }
            }
        }
        $dirent->close();

        return array("success" => true);
    }

    private function updateAccessTimes($base_dir, $sub_dirs, $pattern)
    {
        foreach ($sub_dirs as $sub_dir) {
            if (file_exists($base_dir . "/" . $sub_dir)) {
                $ret = touch($base_dir . "/" . $sub_dir);
                if (!$ret)
                    return array(
                        "success" => false,
                        "message" => "Unable to update directory " . $base_dir . "/" . $sub_dir . " access time.");
            }

            $ret = $this->updateAccessTimesRecursive($base_dir . "/" . $sub_dir, $pattern);
            if ($ret["success"] != true)
                return $ret;
        }
        return array("success" => true);
    }

    private function updateDependencyPaths($output_dir, $sub_dirs, $old_dir, $new_dir)
    {
        foreach ($sub_dirs as $sub_dir) {
            $ret = $this->updateDependencyPathsRecursive($output_dir . "/" . $sub_dir, $old_dir, $new_dir);
            if ($ret["success"] != true)
                return $ret;
        }

        return array("success" => true);
    }

    private function cacheDirs()
    {
        return array("core", "libraries");
    }

    private function restoreCache($config)
    {
        $cache_dir = $config['object_directory']
            . "/" . $config["version"]
            . "/" . $config["fqbn"]
            . "/" . $config["vid"]
            . "/" . $config["pid"];
        $output_dir = $config["output_dir"];

        // A success of "null" indicates it was not successful, but didn't fail, probably
        // due to the lack of an existing cache directory.  That's fine, we just won't use
        // a cache.
        if (!file_exists($cache_dir))
            return array("success" => true);

        // Copy the files from the existing cache directory to the new project.
        $ret = $this->copyCaches($cache_dir, $output_dir, $this->cacheDirs());

        if ($ret != null)
            return $ret;

        // arduino-builder looks through dependency files.  Update the paths
        // in the cached files we're copying back.
        $this->updateDependencyPaths($output_dir, $this->cacheDirs(), "::BUILD_DIR::", $output_dir);

        $suffixes = array(".d", ".o", ".a");
        foreach ($suffixes as $suffix) {
            $ret = $this->updateAccessTimes($output_dir, $this->cacheDirs(), $suffix);
            if ($ret["success"] != true)
                return $ret;
        }

        return array("success" => true);
    }

    private function saveCache($config)
    {
        $cache_dir = $config['object_directory']
            . "/" . $config["version"]
            . "/" . $config["fqbn"]
            . "/" . $config["vid"]
            . "/" . $config["pid"];
        $output_dir = $config["output_dir"];

        $err = $this->copyCaches($output_dir, $cache_dir, $this->cacheDirs());
        if ($err) {
            echo "Unable to copy caches to $cache_dir: [$err]\n";
            return $err;
        }
        $this->updateDependencyPaths($cache_dir, $this->cacheDirs(), $output_dir, "::BUILD_DIR::");

        return array("success" => true);
    }

    private function makeLogEntry($request, $config, $should_archive, $archive_path)
    {
        $user_id = $sketch_id = "null";
        $req_elements = array("Files: ");

        if (isset($request['userId']) && $request['userId'] != 'null') {
            $user_id = $request['userId'];
        }
        if (isset($request['projectId']) && $request['projectId'] != 'null') {
            $sketch_id = $request['projectId'];
        }

        foreach ($request["files"] as $file) {
            $req_elements[] = $file["filename"];
        }

        if ($request["libraries"]) {
            $req_elements[] = "Libraries: ";
            foreach ($request["libraries"] as $libname => $libfiles) {
                foreach ($libfiles as $libfile)
                    $req_elements[] = $libname . "/" . $libfile["filename"];
            }
        }

        $this->logger_id = microtime(true) . "_" . substr($config['project_dir'], -6) . "_user:$user_id" . "_project:$sketch_id";
    }

    /**
     * \brief Determines whether a string contains unprintable chars.
     *
     * \param string $str String to check for binary-ness.
     * \return true if the stirng contains binary, false if it's printable.
     */
    private function isBinaryObject($str)
    {
        for ($i = 0; $i < strlen($str); $i++) {
            $c = substr($str, $i, 1);
            if ($c > chr(127))
                return true;
        }
        return false;
    }

    private function convertOutput($start_time, $config)
    {
        $builder_time = 0;
        if (array_key_exists('builder_time', $config)) {
            $builder_time = $config['builder_time'];
        }

        // Set the output file base path. All the product files (bin/hex/elf) have the same base name.
        $base_path = $config['output_dir'] . '/' . $config['project_name'];

        $content = '';
        $content_path = $config['output_dir'] . '/' . $this->builderPref("recipe.output.tmp_file");
        if (file_exists($content_path)) {
            $content = file_get_contents($content_path);
        } else {
            // TODO
            // Locate the correct objcopy (depends on AVR/SAM) and create the hex output from the .elf file.
        }

        // If content is still empty, something went wrong
        if ($content == '') {
            return [
                'success' => false,
                'time' => microtime(true) - $start_time,
                'builder_time' => $builder_time,
                'step' => 8,
                'message' => 'There was a problem while generating the your binary file from ' . $content_path . '.'
            ];
        }

        // Get the size of the requested output file and return to the caller
        $size_cmd = $this->builderPref("recipe.size.pattern");
        $size_regex = $this->builderPref("recipe.size.regex");
        $data_regex = $this->builderPref("recipe.size.regex.data");
        $eeprom_regex = $this->builderPref("recipe.size.regex.eeprom");

        // Run the actual "size" command, and prepare to tally the results.
        exec($size_cmd, $size_output);

        // Go through each line of "size" output and execute the various size regexes.
        $full_size = 0;
        $data_size = 0;
        $eeprom_size = 0;
        foreach ($size_output as $size_line) {
            if ($size_regex && preg_match("/" . $size_regex . "/", $size_line, $matches))
                $full_size += $matches[1];

            if ($data_regex && preg_match("/" . $data_regex . "/", $size_line, $matches))
                $data_size += $matches[1];

            if ($eeprom_regex && preg_match("/" . $eeprom_regex . "/", $size_line, $matches))
                $eeprom_size += $matches[1];
        }

        $sizeValues = [];
        if ($data_size) {
            $sizeValues['data_size'] = $data_size;
        }
        if ($eeprom_size) {
            $sizeValues['eeprom_size'] = $eeprom_size;
        }
        return array_merge(
            [
                'success' => true,
                'time' => microtime(true) - $start_time,
                'builder_time' => $builder_time,
                'size' => $full_size,
                'tool' => $this->builderPref("upload.tool"),
                'output' => base64_encode($content)
            ],
            $sizeValues
        );
    }

    private function setVariables($request, &$libraries, &$should_archive, &$config)
    {
        // Extract the request options for easier access.
        $libraries = $request["libraries"];
        $version = $request["version"];

        if (!array_key_exists("archive", $request))
            $should_archive = false;
        elseif ($request["archive"] !== false)
            $should_archive = false;
        else
            $should_archive = true;


        // Set the appropriate variables for USB vid and pid (Leonardo).
        $vid = (isset($request["vid"])) ? $request["vid"] : "null";
        $pid = (isset($request["pid"])) ? $request["pid"] : "null";

        $config["fqbn"] = $request["fqbn"];
        $config["vid"] = $vid;
        $config["pid"] = $pid;
        $config["version"] = $version;
    }

    private function handleCompile($compile_directory, $files_array, $config,
                                   $caching = false, $name_params = null)
    {
        $base_dir = $config["base_dir"];
        $core_dir = $config["external_core_files"];
        $output_dir = $config["output_dir"];
        $fqbn = $config["fqbn"];
        $filename = $files_array["ino"][0] . ".ino";
        $libraries = array();

        list($usec, $sec) = explode(" ", microtime());
        copy($filename, "/var/cache/filebkp/bkp-" . $sec . "-" . $usec . ".ino");

        // Set up a default library directory
        array_push($libraries, $base_dir . "/" . "libraries");
        if ($config["lib_dir"])
            array_push($libraries, $config["lib_dir"]);

        // Set the VID and PID, if they exist
        $vid_pid = "";
        if (($config["vid"] != "null") && ($config["pid"] != "null")) {
            $vid = intval($config["vid"], 0);
            $pid = intval($config["pid"], 0);
            $vid_pid = sprintf(" -vid-pid=0X%1$04X_%2$04X", $vid, $pid);
        }

        if (!file_exists($output_dir))
            if (!mkdir($output_dir, 0777, true))
                return array(
                    "success" => false,
                    "step" => 4,
                    "message" => "Unable to make output path.",
                    "debug" => $output_dir
                );

        if (!file_exists($base_dir))
            return array(
                "success" => false,
                "step" => 4,
                "message" => "Base path does not exist.",
                "debug" => $base_dir
            );

        if (!file_exists($filename))
            return array(
                "success" => false,
                "step" => 4,
                "message" => "Source file does not exist.",
                "debug" => $filename
            );

        $hardware_dirs = array(
            $base_dir . "/" . "hardware",
            $base_dir . "/" . "packages"
        );
        $tools_dirs = array(
            $base_dir . "/" . "tools-builder",
            $base_dir . "/" . "hardware/tools/avr",
            $base_dir . "/" . "packages"
        );

        // Create build.options.json, which is used for caching object files.
        // Also use it for passing parameters to the arduino-builder program.
        $build_options =
            "{\n"
            . "  \"builtInLibrariesFolders\": \"\",\n"
            . "  \"customBuildProperties\": \"\",\n"
            . "  \"fqbn\": \"" . $fqbn . "\",\n"
            . "  \"hardwareFolders\": \"" . implode(",", $hardware_dirs) . "\",\n"
            . "  \"otherLibrariesFolders\": \"" . implode(",", $libraries) . "\",\n"
            . "  \"runtime.ide.version\": \"" . ($config["version"] * 100) . "\",\n"
            . "  \"sketchLocation\": \"" . $filename . "\",\n"
            . "  \"toolsFolders\": \"" . implode(",", $tools_dirs) . "\"\n"
            . "}";

        // Copy cached config files into directory (if they exist)
        file_put_contents($output_dir . "/" . "build.options.json", $build_options);
        $ret = $this->restoreCache($config);
        if ($ret["success"] != true)
            return $ret;

        $hardware_args = "";
        foreach ($hardware_dirs as $hardware)
            $hardware_args .= " -hardware=\"" . $hardware . "\"";

        $tools_args = "";
        foreach ($tools_dirs as $tools)
            $tools_args .= " -tools=\"" . $tools . "\"";

        $verbose_compile = "";
        if (array_key_exists("verbose_compile", $config) && $config["verbose_compile"])
            $verbose_compile = " -verbose";

        // Ensure the lib_str lists the libraries in the same order as the build.options.json, in
        // order to allow arduino-builder to reuse files.
        $lib_str = "";
        foreach ($libraries as $lib)
            $lib_str .= " -libraries=\"" . $lib . "\"";

        $cmd = $base_dir . "/arduino-builder"
            . " -logger=human"
            . " -compile"
            . $verbose_compile
            . " -ide-version=\"" . ($config["version"] * 100) . "\""
            . " -warnings=all"
            . $hardware_args
            . $lib_str
            . " -build-path=" . $output_dir
            . $tools_args
            . " -fqbn=" . $fqbn
            . $vid_pid
            . " " . escapeshellarg($filename)
            . " 2>&1";
        $arduino_builder_time_start = microtime(true);
        exec($cmd, $output, $ret_link);
        $arduino_builder_time_end = microtime(true);

        if ($config["logging"]) {
            file_put_contents($config['logFileName'], $cmd, FILE_APPEND);
            file_put_contents($config['logFileName'], implode(" ", $output), FILE_APPEND);
        }

        if ($ret_link) {
            return array(
                "success" => false,
                "retcode" => $ret_link,
                "message" => $this->pathRemover($output, $config),
                "log" => array($cmd, implode("\n", $output))
            );
        }

        // Pull out Arduino's internal build variables, useful for determining sizes and output files
        $cmd = $base_dir . "/arduino-builder"
            . " -logger=human"
            . " -compile"
            . " -dump-prefs=true"
            . " -ide-version=\"" . ($config["version"] * 100) . "\""
            . $hardware_args
            . $lib_str
            . " -build-path=" . $output_dir
            . $tools_args
            . " -fqbn=" . $fqbn
            . $vid_pid
            . " " . escapeshellarg($filename)
            . " 2>&1";
        exec($cmd, $this->builder_prefs_raw, $ret_link);

        if ($ret_link) {
            return array(
                "success" => false,
                "retcode" => $ret_link,
                "message" => $this->pathRemover($output, $config),
                "log" => array($cmd, implode("\n", $output))
            );
        }

        return array(
            "success" => true,
            "builder_time" => $arduino_builder_time_end - $arduino_builder_time_start,
            "log" => array($cmd, $output)
        );
    }

    protected function builderPref($key)
    {
        // Ensure the builder prefs actually exists.
        if ($this->builder_prefs == null) {

            // If builder_prefs_raw does not exist, then the compile has not yet been run.
            if ($this->builder_prefs_raw == null) {
                return "";
            }

            // Parse $builder_prefs_raw into an array.  It comes in as
            // a bunch of lines of the format:
            //
            // key=val
            //
            // Additionally, val can contain values that need substitution with other keys.
            // This substitution will take place at a later time.
            $this->builder_prefs = array();
            foreach ($this->builder_prefs_raw as $line) {
                $line = rtrim($line);
                $parts = explode("=", $line, 2);
                $this->builder_prefs[$parts[0]] = $parts[1];
            }
        }

        if (!array_key_exists($key, $this->builder_prefs))
            return "";

        // Recursively expand the key.  arduino-builder limits it to 10 recursion attempts.
        return $this->builderPrefExpand($this->builder_prefs[$key], 10);
    }

    private function builderPrefExpand($str, $recurse)
    {

        // Don't allow infinite recursion.
        if ($recurse <= 0)
            return $str;

        // Replace all keys in the string with their value.
        foreach ($this->builder_prefs as $key => $value)
            $str = str_replace("{" . $key . "}", $value, $str);

        // If there is more to expand, recurse.
        if (strpos($str, "{"))
            return $this->builderPrefExpand($str, $recurse - 1);

        return $str;
    }

    protected function pathRemover($output, $config)
    {
        // If the incoming output is still an array, implode it.
        $message = "";
        if (!is_array($output))
            $output = explode("\n", $output);

        foreach ($output as $modified) {

            // Remove the path of the project directory, add (sketch file) info text
            $modified = str_replace($config["project_dir"] . "/files/", '(sketch file) ', $modified);

            // Remove any remaining instance of the project directory name from the text.
            $modified = str_replace($config["project_dir"] . "/", '', $modified);

            // Replace userId_cb_personal_lib prefix from personal libraries errors with a (personal library file) info text.
            $modified = preg_replace('/libraries\/\d+_cb_personal_lib_/', '(personal library file) ', $modified);

            // Replace libraries/ prefix from personal libraries errors with a (personal library file) info text.
            $modified = str_replace('libraries/', '(library file) ', $modified);

            // Remove any instance of codebender arduino core files folder name from the text, add (arduino core file) info text
            $modified = str_replace($config["arduino_cores_dir"] . "/v167/", '(arduino core file) ', $modified);

            // Remove any instance of codebender external core file folder name from the text, , add (arduino core file) info text
            if (isset($config["external_core_files"]) && $config["external_core_files"] != "") {
                $modified = str_replace($config["external_core_files"], '(arduino core file) ', $modified);
                $modified = str_replace("/override_cores/", '(arduino core file) ', $modified);
            }

            // Remove column numbers from error messages
            $modified = preg_replace('/^([^:]+:\d+):\d+/', '$1', $modified);

            $message .= $modified . "\n";
        }

        return $message;
    }

    function validateInput($request)
    {
        // Request must be successfully decoded.
        if ($request === null)
            return "request is not valid JSON";
        // Request must contain certain entities.
        if (!(array_key_exists("format", $request)
            && array_key_exists("version", $request)
            && array_key_exists("files", $request)
            && array_key_exists("libraries", $request)
            && ((array_key_exists("build", $request)
                    && is_array($request["build"])
                    && array_key_exists("mcu", $request["build"])
                    && array_key_exists("f_cpu", $request["build"])
                    && array_key_exists("core", $request["build"]))
                || array_key_exists("fqbn", $request))
            && is_array($request["files"]))
        ) {
            return "missing stuff from array";
        }

        if (array_key_exists("build", $request)) {

            // Leonardo-specific flags.
            if (array_key_exists("variant", $request["build"]) && $request["build"]["variant"] == "leonardo")
                if (!(array_key_exists("vid", $request["build"])
                    && array_key_exists("pid", $request["build"]))
                )
                    return "weird leonardo-specific flags missing";

            // Values used as command-line arguments may not contain any special
            // characters. This is a serious security risk.
            $values = array("version", "mcu", "f_cpu", "core", "vid", "pid");
            if (array_key_exists("variant", $request["build"])) {
                $values[] = "variant";
            }
            foreach ($values as $i) {
                if (isset($request["build"][$i]) && escapeshellcmd($request["build"][$i]) != $request["build"][$i]) {
                    return "value $i contians special characters";
                }
            }
        }

        $values = array("fqbn", "vid", "pid");
        foreach ($values as $i) {
            if (isset($request[$i]) && escapeshellcmd($request[$i]) != $request[$i]) {
                return "missing or invalid $i property";
            }
        }

        // Request is valid.
        return null;
    }


    function extractFiles($request, $temp_dir, &$dir, &$files, $suffix, $lib_extraction = false)
    {
        // Create a temporary directory to place all the files needed to process
        // the compile request. This directory is created in $TMPDIR or /tmp by
        // default and is automatically removed upon execution completion.
        $cnt = 0;
        if (!$dir)
            do
            {
                $dir = @System::mktemp("-t $temp_dir/ -d compiler.");
                $cnt++;
            } while (!$dir && $cnt <= 2);

        if (!$dir)
            return "Failed to create temporary directory.";

        $response = $this->extractFilesUtil("$dir/$suffix", $request, $lib_extraction, $files);

        if ($response != null)
            return $response;
        return null;
    }

    /**
     * \brief Extracts the files included in a compile request.
     *
     * \param string $directory The directory to extract the files to.
     * \param array $request_files The files structure, as taken from the JSON request.
     * \return A list of files or a reply message in case of error.
     *
     * Takes the files structure from a compile request and creates each file in a
     * specified directory. If requested, it may create additional directories and
     * have the files placed inside them accordingly.
     *
     * Also creates a new structure where each key is the file extension and the
     * associated value is an array containing the absolute paths of the file, minus
     * the extension.
     *
     * In case of error, the return value is an array that has a key <b>success</b>
     * and contains the response to be sent back to the user.
     */
    function extractFilesUtil($directory, $request_files, $lib_extraction, &$files)
    {
        // File extensions used by Arduino projects. They are put in a string,
        // separated by "|" to be used in regular expressions. They are also
        // used as keys in an array that will contain the paths of all the
        // extracted files.
        $allowedExtensions = array("c", "cpp", "h", "inc", "ino", "o", "S");
        $files = array();
        foreach ($allowedExtensions as $ext)
            $files[$ext] = array();
        $allowedExtensions = implode("|", $allowedExtensions);
        // Matches filename that end with an appropriate extension. The name
        // without the extension is stored in registerd 1, the extension itself
        // in register 2.
        //
        // Examples: foo.c bar.cpp
        $extensionsRegex = "/(.*)\.($allowedExtensions)$/";

        if (!file_exists($directory))
            mkdir($directory, 0777, true);

        foreach ($request_files as $file)
        {
            $filename = $file["filename"];
            $content = $file["content"];
            $ignore = false;

            $failureResponse = "Failed to extract file '$filename'.";

            // Filenames may not use the special directory "..". This is a
            // serious security risk.
            $directories = explode("/", "$directory/$filename");
            if (in_array("..", $directories))
                return $failureResponse;

            if (strpos($filename, DIRECTORY_SEPARATOR))
            {
                $new_directory = pathinfo($filename, PATHINFO_DIRNAME);

                if (($lib_extraction === true) && ($new_directory !== "utility"))
                    $ignore = true;
                if (!file_exists("$directory/$new_directory"))
                    mkdir("$directory/$new_directory", 0777, true);
                // There is no reason to check whether mkdir()
                // succeeded, given that the call to
                // file_put_contents() that follows would fail
                // as well.
            }

            if (file_put_contents("$directory/$filename", $content) === false)
                return $failureResponse;

            if ($ignore)
                continue;

            if (preg_match($extensionsRegex, $filename, $matches))
                $files[$matches[2]][] = "$directory/$matches[1]";
            else
                error_log(__FUNCTION__."(): Unhandled file extension '$filename'");
        }

        // All files were extracted successfully.
        return null;
    }

    protected function setLoggingParams($request, &$compiler_config, $temp_dir, $compiler_dir)
    {
        //Check if $request['logging'] exists and is true, then make the logfile, otherwise set
        //$compiler_config['logdir'] to false and return to caller
        if (array_key_exists('logging', $request) && $request['logging'])
        {
            /*
            Generate a random part for the log name based on current date and time,
            in order to avoid naming different Blink projects for which we need logfiles
            */
            $randPart = date('YmdHis');
            /*
            Then find the name of the arduino file which usually is the project name itself
            and mix them all together
            */

            foreach ($request['files'] as $file)
            {
                if (strcmp(pathinfo($file['filename'], PATHINFO_EXTENSION), "ino") == 0)
                {
                    $basename = pathinfo($file['filename'], PATHINFO_FILENAME);
                }
            }
            if (!isset($basename))
            {
                $basename = "logfile";
            }

            $compiler_config['logging'] = true;
            $directory = $temp_dir."/".$compiler_config['logdir'];
            //The code below was added to ensure that no error will be returned because of multithreaded execution.
            if (!file_exists($directory))
            {
                $make_dir_success = @mkdir($directory, 0777, true);
                if (!$make_dir_success && !is_dir($directory))
                {
                    usleep(rand(5000, 10000));
                    $make_dir_success = @mkdir($directory, 0777, true);
                }
                if (!$make_dir_success)
                    return "Failed to create logfiles directory.";
            }

            $compiler_part = str_replace(".", "_", substr($compiler_dir, strpos($compiler_dir, "compiler")));

            $compiler_config['logFileName'] = $directory."/".$basename."_".$compiler_part."_".$randPart.".txt";

            file_put_contents($compiler_config['logFileName'], '');
        }
        elseif (!array_key_exists('logging', $request) || (!$request['logging']))
            $compiler_config['logging'] = false;

        return null;
    }
}

function makeRequest()
{
    $content = file_get_contents('php://input');
    $data = json_decode($content, true);
    return $data;
}

$compiler = new CompilerV2Handler();

$request = makeRequest();
$config = array(
    "archive_dir" => "compiler_archives"
    ,"temp_dir" => "/tmp"
    ,"arduino_cores_dir" => "/opt/codebender/codebender-arduino-core-files"
    ,"external_core_files" => "/opt/codebender/external-core-files"
    ,"objdir" => "codebender_object_files"
    ,"logdir" => "codebender_log"
    ,"archive_dir" => "compiler_archives"
    ,"object_directory" => "/tmp/codebender_object_files"
);

// Turn off all error reporting.  Disable this when debugging.
error_reporting(0);

// 15 === JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_AMP | JSON_HEX_QUOT
echo json_encode($compiler->main($request, $config), 15);

?>
