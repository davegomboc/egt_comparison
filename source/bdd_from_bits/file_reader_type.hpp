#ifndef EGT_COMPARISON_BDD_FROM_BITS_FILE_READER_TYPE
#define EGT_COMPARISON_BDD_FROM_BITS_FILE_READER_TYPE


#include <cstdio>


using std::fgetc;
using std::fscanf;
using std::ungetc;


//
// Abstract file reader, for compressed vs uncompressed input
//
class file_reader_type {
public:
    /*
      @param  inpath    Input file name, or null to read from stdin
      @param  comp      Type of compression to use:
                          ' ': none
                          'x': xz  compression
                          'g': gz  compression
                          'b': bz2 compression
    */
    file_reader_type(const char * inpath, char comp);

    /*
        Close file properly
    */
    ~file_reader_type();

    inline bool fok() const {
        return F;
    }

    inline bool eof() {
        return feof(F);
    }

    inline int get() {
        return fgetc(F);
    }

    inline unsigned int read_unsigned() {
        unsigned int x;
        fscanf(F, "%u", &x);
        return x;
    }

    inline void unget(int c) {
        ungetc(c, F);
    }

    inline unsigned int read(void * ptr, unsigned int bytes) {
        return static_cast<unsigned int>(fread(ptr, 1, bytes, F));
    }

private:
    FILE * F;
    bool pclose_F;
};


#endif // EGT_COMPARISON_BDD_FROM_BITS_FILE_READER_TYPE
