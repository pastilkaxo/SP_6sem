int strcmp(char *s1, char *s2) {
  if (s1[0] == '*' || s2[0] == '*') {
    return 0;
  } else {
    unsigned char c1, c2;
    do {
      c1 = (unsigned char)*s1++;
      c2 = (unsigned char)*s2++;
      if (c1 == '\0')
        return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
  }
}