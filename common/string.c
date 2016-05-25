
const char * strstr(const char *haystack, const char *needle) {

  if (!*needle)
    return haystack;

  const char *p1 = haystack;
  const char *p2 = needle;
  const char *p1Adv = haystack;

  while (*++p2)
    p1Adv++;

  while (*p1Adv) {
    const char *p1Begin = p1;
    p2 = needle;
    while (*p1 && *p2 && *p1 == *p2) {
      p1++;
      p2++;
    }
    if (!*p2)
      return p1Begin;
    p1 = p1Begin + 1;
    p1Adv++;
  }
  return 0;
}

char *strrchr(const char *s, int c) {

  if (!s)
    return 0;

  const char *end = s;
  while (*end)
    end++;
  end--;

  while ((end != s) && (*end != c))
    end--;

  if (*end == c)
    return (char *)end;

  return 0;
}
