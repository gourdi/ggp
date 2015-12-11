#include <ggo_nonreg.h>
#include <ggo_kernel.h>
#include <ggo_dct.h>
#include <ggo_memory.h>

/////////////////////////////////////////////////////////////////////
GGO_TEST(dct, dct)
{
    float data1[] = {1, 5, 7, 3, 6, 9, 4, 2};
    float data2[8];
    
    ggo::dct(data1, data2, 8);
    ggo::mem_zero(data1, 8);
    ggo::dct(data2, data1, 8);

    GGO_CHECK_FABS(data1[0], 1);
    GGO_CHECK_FABS(data1[1], 5);
    GGO_CHECK_FABS(data1[2], 7);
    GGO_CHECK_FABS(data1[3], 3);
    GGO_CHECK_FABS(data1[4], 6);
    GGO_CHECK_FABS(data1[5], 9);
    GGO_CHECK_FABS(data1[6], 4);
    GGO_CHECK_FABS(data1[7], 2);
}