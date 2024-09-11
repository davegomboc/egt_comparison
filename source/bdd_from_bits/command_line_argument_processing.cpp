#include "bdd_from_bits/command_line_argument_processing.hpp"


#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#include "bdd_from_bits/exception_neutrality.hpp"


// #define EGT_COMPARISON_BDD_FROM_BITS_COMMAND_LINE_ARGUMENT_PROCESSING_DEBUG_PARSE_SWITCHES


// ======================================================================

inline void write_spaces(std::ostream & out, unsigned int i) {
    for (; i; --i) {
        out.put(' ');
    }
}

inline void writestr(std::ostream & out, unsigned int indent, char const * D) {
    for (; *D; D++) {
        out.put(*D);
        if ('\n' == *D) write_spaces(out, indent);
    }
}

// ======================================================================

bdd_option_type::argument_type::argument_type(char const * n, const char* d) {
    name = n;
    doc = d;
    refcount = 1;
}

bdd_option_type::argument_type::~argument_type() {}

void bdd_option_type::argument_type::show(std::ostream & s, unsigned & col) const {
    char const * p = name;
    if (' ' == *p) ++p;
    while (*p) {
        ++col;
        s.put(*p++);
    }
}

void bdd_option_type::argument_type::show_current(std::ostream &, unsigned int) const {}

void bdd_option_type::argument_type::document(std::ostream & out, unsigned int col2) const {
    if (doc) {
        out << "\n";
        write_spaces(out, col2);
        if (! name_hidden()) {
            out << name << " ";
        }
        writestr(out, col2, doc);
    }
}

// ======================================================================

bdd_option_type::bdd_option_type(char const * d, unsigned int na, ...) {
    doc = d;
    numargs = na;
    if (na) {
        A = new argument_type * [numargs];
        va_list argp;
        va_start(argp, na);
        for (unsigned i = 0; i < numargs; ++i) {
            A[i] = va_arg(argp, argument_type *);
        }
        va_end(argp);
    } else {
        A = nullptr;
    }
}

bdd_option_type::~bdd_option_type() {
    for (unsigned i = 0; i < numargs; ++i) {
        if (A[i]) A[i]->unlink();
    }
    delete[] A;
}

unsigned int bdd_option_type::process(char letter, char const ** argp) {
    activate(letter);
    for (unsigned int u = 0; u < numargs; u++) {
        if (A[u]) A[u]->pass(letter, argp[u]);
    }
    return numargs;
}

void bdd_option_type::document(char letter, std::ostream & out, unsigned int ind, unsigned int col2) const {
    write_spaces(out, ind);

    unsigned col1 = ind;
    out << '-' << letter;
    col1 += 2;
    for (unsigned int u = 0; u < numargs; u++) {
        if (A[u]) {
            out << " ";
            col1++;
            A[u]->show(out, col1);
        }
    }

    if (col1 < col2) {
        write_spaces(out, col2-col1);
    } else {
        out << "\n";
        write_spaces(out, col2);
    }

    writestr(out, col2, doc);
    for (unsigned u = 0; u < numargs; u++) {
        if (A[u]) {
            if (!A[u]->name_hidden()) out << "\n";
            A[u]->document(out, col2);
            A[u]->show_current(out, col2);
        }
    }
    show_current(out, col2);
    out << "\n";
}

void bdd_option_type::activate(char) {}

void bdd_option_type::show_current(std::ostream &, unsigned) const {}

// ======================================================================

letter_option_type::letter_option_type(char & st, char const * doc)
  : bdd_option_type(doc, 0), state(st) {
    actletter = 0;
}

letter_option_type::~letter_option_type() {}

void letter_option_type::activate(char letter) {
    state = letter;
    actletter = letter;
}

void letter_option_type::show_current(std::ostream & out, unsigned int col2) const {
    if (actletter && actletter == state) {
        out << "\n";
        write_spaces(out, col2);
        out << "Currently set.";
    }
}

// ======================================================================

string_argument_type::string_argument_type(
    char const * nam,
    char const * & st,
    char const * dc
) : argument_type(nam, dc), state(st) {}

string_argument_type::~string_argument_type() {}

void string_argument_type::show_current(std::ostream & out, unsigned int col2) const {
    if (state) {
        out << "\n";
        write_spaces(out, col2);
        if (name_hidden()) {
            out << "Currently set to " << state;
        } else {
            out << name << " is currently: " << state;
        }
    }
}

void string_argument_type::pass(char L, char const * arg) {
    if (nullptr == arg) {
        std::cerr << "Error: string expected for " << name << " after -" << L << "\n";
        exit(1);
    }
    state = arg;
}

// ======================================================================

enum_argument_type::enum_argument_type(
    char const * nam,
    unsigned int & st,
    char const * dc,
    unsigned int nv, ...
) : argument_type(nam, dc), state(st) {
    numvals = nv;
    if (numvals) {
        valstrings = new value_type * [numvals];
        va_list argp;
        va_start(argp, nv);
        for (unsigned u = 0; u < numvals; ++u) {
            valstrings[u] = va_arg(argp, value_type *);
        }
        va_end(argp);
    } else {
        valstrings = nullptr;
    }
}

enum_argument_type::~enum_argument_type() {
    for (unsigned u = 0; u < numvals; ++u) {
        if (valstrings[u]) valstrings[u]->unlink();
    }
    delete[] valstrings;
}

void enum_argument_type::show_current(std::ostream & out, unsigned int col2) const {
    out << "\n";
    write_spaces(out, col2);
    if (name_hidden()) {
        out << "Currently set to ";
    } else {
        out << name << " is currently: ";
    }
    out << valstrings[state]->valstr;
}

void enum_argument_type::pass(char L, char const * arg) {
    if (nullptr == arg) {
        std::cerr << "Error: string expected for " << name << " after -" << L << "\n";
        exit(1);
    }
    for (unsigned u = 0; u < numvals; ++u) {
        if (0 == strcmp(valstrings[u]->valstr, arg)) {
            state = u;
            return;
        }
    }
    std::cerr << "Error: unknown value " << arg << " for " << name << " after -" << L << "\n";
    exit(1);
}

void enum_argument_type::document(std::ostream & out, unsigned col2) const {
    argument_type::document(out, col2);

    unsigned int maxlen = 0;
    for (unsigned int u = 0; u < numvals; ++u) {
        unsigned int ulen = static_cast<unsigned int>(strlen(valstrings[u]->valstr));
        if (ulen > maxlen) maxlen = ulen;
    }

    out << "\n";
    write_spaces(out, col2);
    out << "Options";
    if (not name_hidden()) {
        out << " for " << name;
    }
    out << " are:";

    for (unsigned int u = 0; u < numvals; ++u) {
        out << "\n";
        unsigned int ulen = static_cast<unsigned int>(strlen(valstrings[u]->valstr));
        write_spaces(out, col2 + 2);
        out << valstrings[u]->valstr;
        write_spaces(out, 2 + maxlen - ulen);
        writestr(out, col2 + maxlen + 4, valstrings[u]->doc);
    }
}

// ======================================================================

unsigned_argument_type::unsigned_argument_type(
    char const * nam,
    unsigned int & U,
    char const * dc
) : argument_type(nam, dc), state(U) {}

unsigned_argument_type::~unsigned_argument_type() {}

void unsigned_argument_type::show_current(std::ostream & out, unsigned int col2) const {
    out << "\n";
    write_spaces(out, col2);
    if (name_hidden()) {
        out << "Currently set to " << state;
    } else {
        out << name << " is currently: " << state;
    }
}

void unsigned_argument_type::pass(char L, char const * arg) {
    if (nullptr == arg) {
        std::ostringstream error_message{"Error: integer expected for"};
	error_message << name << " after -" << L;
	bdd_terminate(error_message.str());
    }
    unsigned int ans = 0;
    while (*arg) {
        if ((*arg < '0') or ('9' < *arg)) {
            std::ostringstream error_message{"Error: integer expected for"};
            error_message << name << " after -" << L;
            bdd_terminate(error_message.str());
        }
        ans *= 10;
        ans += unsigned(*arg - '0');
        arg++;
    }
    state = ans;
}

// ======================================================================

signed_argument_type::signed_argument_type(
    char const * nam,
    int & I,
    char const * dc
) : argument_type(nam, dc), state(I) {}

signed_argument_type::~signed_argument_type() {}

void signed_argument_type::show_current(std::ostream & out, unsigned int col2) const {
    out << "\n";
    write_spaces(out, col2);
    if (name_hidden()) {
        out << "Currently set to " << state;
    } else {
        out << name << " is currently: " << state;
    }
}

void signed_argument_type::pass(char L, char const * arg) {
    if (nullptr == arg) {
        std::ostringstream error_message{"Error: integer expected for"};
	error_message << name << " after -" << L;
	bdd_terminate(error_message.str());
    }
    unsigned int const start = ('-' == arg[0]) ? 1 : 0;
    for (unsigned int i = start; arg[i]; i++) {
        if ((*arg < '0') or ('9' < *arg)) {
            std::ostringstream error_message{"Error: integer expected for"};
            error_message << name << " after -" << L;
            bdd_terminate(error_message.str());
        }
    }
    state = static_cast<int>(atol(arg));
}

// ======================================================================

options_manager_type::options_manager_type(char help, char const * usage) {
    help_letter = help;
    usage_header = usage;
    set_col1(4);
    set_col2(15);

    optarray = new bdd_option_type * [128];
    for (unsigned i = 0; i < 128; i++) optarray[i] = nullptr;
}

options_manager_type::~options_manager_type() {
    for (unsigned i = 0; i < 128; i++) {
        delete optarray[i];
    }
    delete[] optarray;
}

bool options_manager_type::add_option(char letter, bdd_option_type * o) {
    if (nullptr == o)               return false;
    if (letter<0)                   return false;
    // if (letter > 127)               return false;
    if (letter == help_letter)      return false;
    if (optarray[unsigned(letter)]) return false;
    optarray[unsigned(letter)] = o;
    return true;
}

int options_manager_type::parse_switches(int argc, char ** argv) {
    unsigned int i;
    for (i = 1; i < unsigned(argc); ) {
#ifdef EGT_COMPARISON_BDD_FROM_BITS_COMMAND_LINE_ARGUMENT_PROCESSING_DEBUG_PARSE_SWITCHES
        std::cerr << "Checking argument: '" << argv[i] << "'\n";
#endif
        if (0   == argv[i][0]) return static_cast<int>(i);
        if ('-' != argv[i][0]) return static_cast<int>(i);
        if (0   == argv[i][1]) return static_cast<int>(i);
        if (0   != argv[i][2]) return static_cast<int>(i);
        char const letter = argv[i][1];
        if (help_letter == letter) {
            show_usage(std::cerr, argv[0]);
            exit(1);
        }
        bdd_option_type * o = optarray[unsigned(letter)];
        if (!o) return static_cast<int>(i);
        ++i;
        i += o->process(letter, const_cast<char const **>(argv + i));
    }
    return static_cast<int>(i);
}

void options_manager_type::show_usage(std::ostream & out, char const * exe) const {
    char const * baseexe = exe;
    for ( ; *exe; exe++) {
        if ('/' == *exe) {
            baseexe = exe + 1;
        }
    }
    out << "Usage: " << baseexe << " " << usage_header;
    out << "\nOptions:\n";

    for (char u = 0; u < 127; ++u) {
        if ((u >= 'a') && (u <= 'z')) continue;
        if ((u >= 'A') && (u <= 'Z')) {
            document_letter(out, u + 32);
        }
        document_letter(out, u);
    }
}

void options_manager_type::document_letter(std::ostream & out, char L) const {
    if (help_letter == L) {
        write_spaces(out, col1pos);
        out << '-' << help_letter;
        write_spaces(out, col2pos - (col1pos + 2u));
        writestr(out, col2pos, "This help");
        out << "\n";
    } else if (optarray[int(L)]) {
        optarray[int(L)]->document(L, out, col1pos, col2pos);
    } else {
        return;
    }
    out << "\n";
}
