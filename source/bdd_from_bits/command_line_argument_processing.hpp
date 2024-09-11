#ifndef EGT_COMPARISON_BDD_FROM_BITS_COMMAND_LINE_ARGUMENT_PROCESSING_HPP
#define EGT_COMPARISON_BDD_FROM_BITS_COMMAND_LINE_ARGUMENT_PROCESSING_HPP


#include <ostream>

// ======================================================================

class bdd_option_type {
public:
    class argument_type;

    bdd_option_type(char const * doc, unsigned int numargs, ...);
    virtual ~bdd_option_type();

    unsigned int process(char letter, char const ** argp);

    /**
      Document this option.
    */
    void document(char letter, std::ostream & out, unsigned int indent, unsigned int col2) const;

    virtual void activate(char letter);

    virtual void show_current(std::ostream & out, unsigned int col2) const;

private:
    unsigned int numargs;
    argument_type ** A;
    char const * doc;
};


class bdd_option_type::argument_type {
public:
    argument_type(char const * name, char const * doc);

    void show(std::ostream & s, unsigned int & col) const;
    virtual void show_current(std::ostream & out, unsigned int col2) const;

    // Write to stderr and exits, on error
    virtual void pass(char L, char const * arg) = 0;

    // Document legal values if necessary
    virtual void document(std::ostream & out, unsigned int col2) const;

    inline argument_type * link() {
        ++refcount;
        return this;
    }
    void unlink() {
        --refcount;
        if (0 == refcount) delete this;
    }

    inline bool name_hidden() const {
        return (name[0] == ' ');
    }

protected:
    char const * name;
    char const * doc;
    virtual ~argument_type();

private:
    unsigned int refcount;
};


// ======================================================================

class letter_option_type : public bdd_option_type {
public:
    letter_option_type(char & state, char const * doc);
    virtual ~letter_option_type() override;

    virtual void activate(char letter) override;
    virtual void show_current(std::ostream& out, unsigned int col2) const override;

private:
    char & state;
    char actletter;
};

// ======================================================================

class string_argument_type : public bdd_option_type::argument_type {
public:
    string_argument_type(char const * name, char const * & st, char const * doc);
    virtual ~string_argument_type() override;

    virtual void show_current(std::ostream & out, unsigned int col2) const override;
    virtual void pass(char L, char const * arg) override;
private:
    char const * & state;
};

// ======================================================================

class enum_argument_type : public bdd_option_type::argument_type {
public:
    class value_type;

    enum_argument_type(char const * name, unsigned int & st, char const * doc, unsigned int numvals, ...);
    virtual ~enum_argument_type() override;

    virtual void show_current(std::ostream & out, unsigned int col2) const override;
    virtual void pass(char L, char const * arg) override;
    virtual void document(std::ostream & out, unsigned int col2) const override;

private:
    unsigned int & state;
    unsigned int numvals;
    value_type ** valstrings;
};


class enum_argument_type::value_type {
public:
    value_type() {
        refcount = 1;
    }

    value_type(char const * v, char const * d) {
        refcount = 1;
        set(v, d);
    }

    char const * get_str() const {
        return valstr;
    }

    inline void set(char const * v, char const * d) {
        valstr = v;
        doc = d;
    }

    inline value_type * link() {
        ++refcount;
        return this;
    }

    inline void unlink() {
        --refcount;
        if (0 == refcount) delete this;
    }

private:
    friend class enum_argument_type;

    unsigned int refcount;
    char const * valstr;
    char const * doc;
};

// ======================================================================

class unsigned_argument_type : public bdd_option_type::argument_type {
public:
    unsigned_argument_type(char const * name, unsigned int & U, char const * doc);
    virtual ~unsigned_argument_type() override;

    virtual void show_current(std::ostream &out, unsigned int col2) const override;
    virtual void pass(char L, const char* arg) override;
private:
    unsigned int & state;
};

// ======================================================================

class signed_argument_type : public bdd_option_type::argument_type {
public:
    signed_argument_type(char const * name, int & I, char const * doc);
    virtual ~signed_argument_type() override;

    virtual void show_current(std::ostream & out, unsigned int col2) const override;
    virtual void pass(char L, char const * arg) override;
private:
    int & state;
};

// ======================================================================

class options_manager_type {
public:
    options_manager_type(char help, char const * usage);
    ~options_manager_type();

    inline void set_col1(unsigned char col1) {
        col1pos = col1;
    }

    inline void set_col2(unsigned char col2) {
        col2pos = col2;
    }

    bool add_option(char letter, bdd_option_type *);

    /**
      Process command line arguments until no more matches.
      Returns index of first non-processed argument.
    */
    int parse_switches(int argc, char ** argv);

    void show_usage(std::ostream & out, char const * exe) const;

private:
    void document_letter(std::ostream & out, char L) const;

    bdd_option_type ** optarray;
    char const * usage_header;
    char help_letter;
    unsigned char col1pos, col2pos;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_COMMAND_LINE_ARGUMENT_PROCESSING_HPP
