#pragma once
#include <memory>
namespace utility
{
template<class T>
using hard_pointer = std::shared_ptr<T>;
template<class T>
using hardptr = hard_pointer<T>;
template<class T, class ...ARGS>
hard_pointer<T> make_hard_pointer( ARGS ...args )
{
    return std::make_shared<T>( args... );
}

// weak_ptrを使用するときに、いちいち lock とか expired を使うのがだるいのでラッピングします。

template<class T>
class soft_pointer : protected std::weak_ptr<T>
{
public:
    soft_pointer( ) = default;
    soft_pointer( hard_pointer<T> const& right ) noexcept
        : std::weak_ptr<T>( right )
    {
    }
    soft_pointer( hard_pointer<T>&& right ) noexcept
        : std::weak_ptr<T>( std::move( right ) )
    {
    }
    template<class T2>
    soft_pointer& operator=( soft_pointer<T2> const& right ) noexcept
    {
        this->_Resetw( right.lock( ) );
        return ( *this );
    }
    template<class T2>
    soft_pointer& operator=( hard_pointer<T2> const& right ) noexcept
    {
        this->_Resetw( right );
        return ( *this );
    }
    operator bool( ) const noexcept // ポインタがnullや期限切れを判断します。
    {
        return this->lock( ) && this->expired( );
    }
    bool operator!( ) const noexcept // ポインタにnotを与えます。
    {
        return !this->lock( );
    }
    hard_pointer<T> operator->( ) const noexcept // ポインタの実態を返します。
    {
        return this->lock( );
    }
    hard_pointer<T> operator*( ) const noexcept
    {
        return this->lock( );
    }
    bool operator==( soft_pointer<T> const& right ) const noexcept // ポインタ同士を比較します。
    {
        return this->lock( ) == right.lock( );
    }
    bool operator!=( soft_pointer<T> const& right ) const noexcept
    {
        return this->lock( ) != right.lock( );
    }
    bool operator==( hard_pointer<T> const& right ) const noexcept
    {
        return this->lock( ) == right;
    }
    bool operator!=( hard_pointer<T> const& right ) const noexcept
    {
        return this->lock( ) != right;
    }
    operator T( ) const noexcept // 自動的にポインタの実態を返します。
    {
        return *this->lock( );
    }
    operator T&( ) const noexcept
    {
        return *this->lock( );
    }
    operator T const&( ) const noexcept
    {
        return *this->lock( );
    }
};
template<class T>
using softptr = soft_pointer<T>;
}
