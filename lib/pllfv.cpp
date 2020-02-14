// Laboratory work 1 on the discipline LOIS
// Completed by student of group 721702 BSUIR Komar Alexander Nazarovich
// CPP-file of library of propositional logic language formula verifier
// Version 4. Merge compiling and verifying in one function. Add exception class
// for pcre compilation. Cleaning code
//
// www.pcre.org
// https://www.opennet.ru/base/dev/pcre_cpp.txt.html
// https://eax.me/libpcre/
//

#include "pllfv.h"
#include <pcre.h>

bool matchRegexp(const char *pattern, const char *str, int len) {
    // compile pattern to regular
    const char* error;
    int error_offset;
    pcre *regular = pcre_compile(pattern, 0, &error, &error_offset, nullptr);
    if (regular == nullptr) {
        //free regexp memory
        pcre_free((void *) regular);
        throw PcreCompilationException();
    }
    // verify str
    int count = 0;
    int offsets = 30;  // should be multiple of 3
    int offsets_vector[offsets];
    count = pcre_exec(regular, nullptr, str, len, 0, 0, offsets_vector, offsets);
    // free regexp memory
    pcre_free((void *) regular);
    return (count == 1) &&                                      // only one match
           (offsets_vector[0] == 0) &&                          // and it is from str start
           (offsets_vector[1] == len);                          // to str end
}

const char *PcreCompilationException::what() const noexcept {
    return "PCRE compilation failed";
}
