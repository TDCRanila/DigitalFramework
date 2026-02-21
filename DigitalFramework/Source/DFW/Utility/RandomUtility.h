#pragma once

#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/mersenne_twister.hpp>

namespace DFW
{
    namespace DUtility
    {
        namespace Detail
        {
            inline boost::random::mt19937 DFW_RANDOM_NUMBER_GENERATOR;

        } // End of namespace ~ Detail.

        // Outputs either 0 or 1.
        inline int32 Random01()
        {
            boost::random::uniform_int_distribution<> distribution(0, 1);
            return distribution(Detail::DFW_RANDOM_NUMBER_GENERATOR);
        }

        inline bool RandomBool()
        {
            return Random01();
        }

        // Integer Range [0, IntType::max]
        template <typename IntType = int32>
        inline IntType RandomInt()
        {
            boost::random::uniform_int_distribution<> distribution;
            return distribution(Detail::DFW_RANDOM_NUMBER_GENERATOR);
        }

        template <typename IntType = int32>
        inline IntType RandomIntRange(IntType const a_minimum, IntType const a_maximum)
        {
            boost::random::uniform_int_distribution<> distribution(a_minimum, a_maximum);
            return distribution(Detail::DFW_RANDOM_NUMBER_GENERATOR);
        }

        // Floating Point Range [0.0, 1.0]
        template <typename FloatType = float32>
        inline FloatType RandomFloat()
        {
            boost::random::uniform_01<FloatType, FloatType> distribution;
            return distribution(Detail::DFW_RANDOM_NUMBER_GENERATOR);
        }

        template <typename FloatType = float32>
        inline FloatType RandomFloatRange(FloatType const a_minimum, FloatType const a_maximum)
        {
            boost::random::uniform_real_distribution<> distribution(a_minimum, a_maximum);
            return distribution(Detail::DFW_RANDOM_NUMBER_GENERATOR);
        }

    } // End of namespace ~ DUtility.

} // End of namespace ~ DFW.
