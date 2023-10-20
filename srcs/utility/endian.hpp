#ifndef ENDIAN_HPP
#define ENDIAN_HPP

typedef enum E_ENDIAN
{
    LITTLE=0,
    BIG=1,
    INVALID=2,
} ENDIAN;

class Endian
{
    public:
        ~Endian();
        static Endian *get_instance();
        static ENDIAN get_endian();
        static ENDIAN endian;
    private:
        static void detect_endian();
        static Endian *singleton;
        Endian();
};

#endif

