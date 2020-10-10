#include <stdio.h>
#include <openipDS/DigitalAlphabet.h>

namespace openip
{
    DigitalAlphabet::DigitalAlphabet()
    {
        this->width= 4;
        this->height= 5;

        this->init(this->a, "01101001111110011001\0");
        this->init(this->b, "11101001111010011110\0");
        this->init(this->c, "01111000100010000111\0");
        this->init(this->d, "11101001100110011110\0");
        this->init(this->e, "11111000111110001111\0");
        this->init(this->f, "11111000111110001000\0");
        this->init(this->g, "01111000101110010111\0");
        this->init(this->h, "10011001111110011001\0");
        this->init(this->i, "01000100010001000100\0");
        this->init(this->j, "00010001000110010110\0");
        this->init(this->k, "10011010110010101001\0");
        this->init(this->l, "10001000100010001110\0");
        this->init(this->m, "10011111100110011001\0");
        this->init(this->n, "10011101101110011001\0");
        this->init(this->o, "01101001100110010110\0");
        this->init(this->p, "11101001111010001000\0");
        this->init(this->q, "01101001100110100101\0");
        this->init(this->r, "11101001111010101001\0");
        this->init(this->s, "01111000011000011110\0");
        this->init(this->t, "11100100010001000100\0");
        this->init(this->u, "10011001100110010110\0");
        this->init(this->v, "10011001011001100100\0");
        this->init(this->w, "11011101110101100110\0");
        this->init(this->x, "10011001011010011001\0");
        this->init(this->y, "10011001111100011110\0");
        this->init(this->z, "11110001011010001111\0");
        this->init(this->d0, "01101011101111010110\0");
        this->init(this->d1, "01001100010001001110\0");
        this->init(this->d2, "01101001001001001111\0");
        this->init(this->d3, "11100001011000011110\0");
        this->init(this->d4, "00110101100111110001\0");
        this->init(this->d5, "11111000111100011110\0");
        this->init(this->d6, "01101000111010010110\0");
        this->init(this->d7, "11110001001000100010\0");
        this->init(this->d8, "01101001011010010110\0");
        this->init(this->d9, "01101001011100010110\0");
        this->init(this->sc, "00000000000000100010\0");
        this->init(this->sd, "00000000000000000010\0");
        this->init(this->ss, "00000000000000000000\0");
    }

    void DigitalAlphabet::init(std::vector<unsigned char>& p, const char* str)
    {
        int i= 0;
        while( str[i] )
        {
            p.push_back(str[i] - '0');
            ++i;
        }
    }

    std::ostream& operator<<(std::ostream& o, const DigitalAlphabet& d)
    {
        o << "[DigitalAlphabet - size of letters: ";
        o << d.width;
        o << "x";
        o << d.height;
        o << "]";

        return o;
    }
}
