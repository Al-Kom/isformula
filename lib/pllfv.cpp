// Laboratory work 1 on the discipline LOIS
// Completed by student of group 721702 BSUIR Komar Alexander Nazarovich
// CPP-file of library of propositional logic language formula verifier
// Version 3. Using PCRE. Moving from main-file to separated library.
//
// www.pcre.org
// https://www.opennet.ru/base/dev/pcre_cpp.txt.html
// https://eax.me/libpcre/
//

#include <pcre.h>
#include <cstring>

pcre *compileRegexp(const char *pattern) {
    const char* error;
    int error_offset;
    return pcre_compile(pattern, 0, &error, &error_offset, nullptr);
}

bool matchRegexp(const pcre *regular, const char *str, int len) {
    int count = 0;
    int offsets = 30;  // should be multiple of 3
    int offsets_vector[offsets];
    count = pcre_exec(regular, nullptr, str, len, 0, 0, offsets_vector, offsets);
    return (count == 1) &&                                      // only one match
           (offsets_vector[0] == 0) &&                          // and it is from str start
           (offsets_vector[1] == strlen(str));                  // to str end
}
