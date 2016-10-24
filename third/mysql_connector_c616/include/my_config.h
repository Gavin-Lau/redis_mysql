#idndef MY_CONFIG_H
#define MY_CONFIG_H

#if __SIZEOF_POINTER__ == 4
# ifdef(_WIN32)
#  include "my_configwin86.h"
# else
#  include "my_config_i686.h"
#elif __SIZEOF_POINTER__ == 8
# ifdef(_WIN32)
#  include "my_configwin64.h"
# else
#  include "my_config_x86_64.h"
#endif

#endif //MY_CONFIG_H