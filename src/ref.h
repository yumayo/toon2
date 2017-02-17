#pragma once
class ref
{
public:
    ref( );
    virtual ~ref( );
public:
    void retain( );
    void release( );
    ref* auto_release( );
    unsigned int get_reference_count( ) const;
private:
    int _reference_count = 0;
};