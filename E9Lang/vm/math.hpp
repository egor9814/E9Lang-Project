//
// Created by egor9814 on 01.04.18.
//

#ifndef E9LANG_PROJECT_MATH_HPP
#define E9LANG_PROJECT_MATH_HPP

#include <string>
#include <vector>

namespace e9lang {

    namespace math {

        double pow(double value, double power);


        void static_init();
        void static_finalize();


        class BigInt;

        class BigFloat;


        class BigInt {
        public:
            typedef std::vector<int> magnitude_t;

        private:
            int signum;
            magnitude_t mag;

        public:
            static BigInt *Zero, *One;

            explicit BigInt(const std::string& value);

            BigInt(const magnitude_t& mag, int signum);


            magnitude_t& getMagnitude();

            int getSignum() const;


            BigInt operator+(const BigInt &val);

            BigInt operator-(const BigInt &val);

            BigInt operator*(const BigInt &val);

            BigInt operator/(const BigInt &val);

            BigInt operator_pow(BigFloat &val);

            BigInt operator%(const BigInt &val);


            BigInt operator&(const BigInt &val);

            BigInt operator|(const BigInt &val);

            BigInt operator^(const BigInt &val);

            BigInt operator>>(const BigInt &val);

            BigInt operator<<(const BigInt &val);


            BigInt operator==(const BigInt &val);

            BigInt operator!=(const BigInt &val);

            BigInt operator<(const BigInt &val);

            BigInt operator<=(const BigInt &val);

            BigInt operator>(const BigInt &val);

            BigInt operator>=(const BigInt &val);


            BigInt operator!();

            BigInt operator~();

            BigInt operator_negate();


            BigInt square();

            BigInt abs();


            std::string toString();


            BigFloat toFloat();
        };


        class MutableBigInt {
            friend class BigInt;

            BigInt::magnitude_t val;
            size_t len;
            size_t offset = 0;

            static MutableBigInt *One;

            MutableBigInt();

            explicit MutableBigInt(int value);

            explicit MutableBigInt(const BigInt::magnitude_t &mag);

            explicit MutableBigInt(const BigInt &value);

            MutableBigInt(const MutableBigInt &value);

            void ones(size_t n);

            BigInt::magnitude_t getMagnitude();

            long toLong();

            BigInt toBigInt(int sign);

            BigInt toBigInt();

            long toComactValue(int sign);

            void clear();

            void reset();

            int compare(const MutableBigInt &x);

            int compareShifted(const MutableBigInt &x, size_t ints);

        };
    }

}

#endif //E9LANG_PROJECT_MATH_HPP
