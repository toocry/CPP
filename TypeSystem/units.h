#ifndef UNITS_H_
#define UNITS_H_

#include <iostream>

template <int ... Values>
struct static_vector
{};

template <typename Vector, int Added>
struct push_back
{};

template<int Elem, typename Vector>
struct push_front
{};

template <int Added, int ... Values>
struct push_back<static_vector<Values ...>, Added>
{
    using type = static_vector<Values ..., Added>;
};


template<int Elem, int ... Values>
struct push_front< Elem, static_vector<Values ...>>
{
    using type = static_vector<Elem, Values ...>;
};


template <typename Vector, size_t Index>
struct at
{};

template <int Head, int ... Tail>
struct at<static_vector<Head, Tail ...>, 0>
{
    static constexpr auto value = Head;
};

template <size_t Index, int Head, int ... Tail>
struct at<static_vector<Head, Tail ...>, Index>
{
    static constexpr auto value = at<static_vector<Tail ...>, Index - 1>::value;
};



template <int Size, int Index>
struct create_vector
{};

template <int Index>
struct create_vector<0, Index>
{
    using type = static_vector<> ;
};

template <int Size>
struct create_vector<Size, 0>
{
    using type = typename push_front<1,typename create_vector<Size - 1, -1>::type>::type;
};

template <int Index, int Size>
struct create_vector<Size, Index>
{
    using type = typename push_front<0,typename create_vector<Size - 1, Index - 1>::type>::type;
};


template<typename TEnum, typename TPowers>
struct unit 
{
    using domain = TEnum;
    using dim_of_units = TPowers; 
};


template<typename Vec1, typename Vec2>
struct sum 
{};

template<typename Vec1, typename Vec2>
struct minus_
{};

template<int Head, int Head1>
struct sum<static_vector<Head>,static_vector<Head1>>
{
    using type = static_vector<Head + Head1>;
};

template<int Head, int Head1>
struct minus_<static_vector<Head>,static_vector<Head1>>
{
    using type = static_vector<Head - Head1>;
};


template <int Head, int Head1, int ... Tail, int ... Tail1>
struct sum<static_vector<Head, Tail ...>,static_vector<Head1, Tail1 ...>>
{       
    using type = typename push_front< Head + Head1,typename sum<static_vector<Tail ...>,static_vector<Tail1 ...>>::type > :: type;

};

template <int Head, int Head1, int ... Tail, int ... Tail1>
struct minus_<static_vector<Head, Tail ...>,static_vector<Head1, Tail1 ...>>
{       
    using type = typename push_front< Head - Head1,typename minus_<static_vector<Tail ...>,static_vector<Tail1 ...>>::type > :: type;

};


template <typename Unit, typename ... Unit1>
struct mult_unit 
{};

template<typename Unit>
struct mult_unit<Unit>
{
    using type = Unit;
};

template<typename Unit1, typename  Unit>
struct mult_unit<Unit, Unit1>
{
    
    using type = unit< typename Unit::domain, typename sum<typename Unit::dim_of_units, typename Unit1:: dim_of_units>::type>;
};


template <typename ... Tail, typename Head>
struct mult_unit<Head, Tail ...>
{
    using type = typename mult_unit<Head, typename mult_unit<Tail ...>::type>::type;
};


template <typename TDividendUnit, typename TDivisorUnit>
struct divide_
{};

template <typename TDivisorUnit, typename TDividendUnit>
struct divide_<TDividendUnit, TDivisorUnit>
{   
    using type = unit<typename TDividendUnit::domain, typename minus_<typename TDividendUnit::dim_of_units, typename TDivisorUnit::dim_of_units>::type>;
};



template <typename TEnum, TEnum Index>
using basic_unit = unit<TEnum, typename create_vector<int(TEnum::_count), int(Index)>::type>;


template <typename TFirstUnit, typename ... TOtherUnits>
using multiplied_unit = typename mult_unit<TFirstUnit,TOtherUnits ... >::type;


template <typename TDividendUnit, typename TDivisorUnit>
using divided_unit = typename divide_<TDividendUnit,TDivisorUnit>::type;

template <typename TUnit, typename TValue = double>
struct quantity{};


template<typename TEnum, typename TValue, typename TPowers >
struct quantity<unit<TEnum,TPowers>, TValue>
{

    explicit quantity(const TValue& value) : _value(value){};

    TValue value() { return _value; }

    TValue _value;

    auto operator+(quantity<unit<TEnum,TPowers>, TValue>& a)
        {return quantity (a.value() + this->value());}
    auto operator-(quantity<unit<TEnum,TPowers>, TValue>& a)
        {return quantity (a.value() - this->value());}
    
    template<typename TPowers1>
    auto operator*(quantity<unit<TEnum,TPowers1>,TValue> &a)
        {return quantity<multiplied_unit<unit<TEnum,TPowers>,unit<TEnum, TPowers1>>>(this->value() * a.value());} 
    template<typename TPowers1>
    auto operator/(quantity<unit<TEnum,TPowers1>,TValue> &a)
        {return quantity<divided_unit<unit<TEnum,TPowers>,unit<TEnum, TPowers1>>>(this->value() / a.value());}      
};

#endif