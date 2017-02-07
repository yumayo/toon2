#ifdef l_class
#define l_prop(__name) #__name, sol::property( &l_class::get_##__name, &l_class::set_##__name )
#define l_readonly(__name) #__name, sol::readonly_property( &l_class::get_##__name )
#define l_set(__name) #__name, &l_class::__name
#define l_new(__name, ...) lua.new_usertype<l_class>( #__name, __VA_ARGS__ );
#define l_base(__name) sol::base_classes, sol::bases<__name>( )
#endif