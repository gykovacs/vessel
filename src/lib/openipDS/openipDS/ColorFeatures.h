#ifndef COLORFEATURE2S_H
#define COLORFEATURE2S_H

#include <openipDS/Feature2.h>

namespace openip
{
    template<typename INPUT, typename OUTPUT>
    class ChannelFeature2: public Feature2<INPUT, OUTPUT>
    {
    public:
        /**
         * constructor
         * @param r radius of the feature
         * @param stride stride of the images to apply to
         */
        ChannelFeature2(int channel);

        /**
         * computes the feature in a given position of the input image
         * @param input input image
         * @param n the position
         * @return the computed feature
         */
        OUTPUT apply(Image<INPUT>& input, int n, Image<unsigned char>* support= NULL);

        int channel;
    };

    template<typename INPUT, typename OUTPUT>
    ChannelFeature2<INPUT, OUTPUT>::ChannelFeature2(int channel)
    {
        std::stringstream ss;
        ss << "ChannelFeature2 " << channel;
        descriptor= ss.str();
    }

    template<typename INPUT, typename OUTPUT>
    OUTPUT ChannelFeature2<INPUT, OUTPUT>::apply(Image<INPUT>& input, int n, Image<unsigned char>* support)
    {

    }
}

#endif // COLORFEATURE2S_H
