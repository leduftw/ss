#ifndef DIRECTIVE_H
#define DIRECTIVE_H

class Directive {
public:
    enum {
        GLOBAL,
        EXTERN,
        SECTION,
        WORD,
        SKIP,
        EQU,
        END,
    };
};

#endif
