#ifndef __GGO_ASSERT__
#define __GGO_ASSERT__

#ifdef GGO_DEBUG
#ifdef WIN32
void GGO_ASSERT(bool b);
#else
#include <assert.h>
#define	GGO_ASSERT(zzz) assert(zzz)
#endif
#define	GGO_TRACE(...) printf(__VA_ARGS__)
#define	GGO_ASSERT_PTR(ptr) if (!ptr) { std::cerr << std::endl << #ptr << " is null" << std::endl; GGO_FAIL(); }
#define	GGO_ASSERT_EQ(v1, v2) if ((v1) != (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] != " << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
#define	GGO_ASSERT_NE(v1, v2) if ((v1) == (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] == " << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
#define	GGO_ASSERT_LT(v1, v2) if ((v1) >= (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] >= " << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
#define	GGO_ASSERT_LE(v1, v2) if ((v1) >  (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] > "  << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
#define	GGO_ASSERT_GT(v1, v2) if ((v1) <= (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] <= " << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
#define	GGO_ASSERT_GE(v1, v2) if ((v1) <  (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] < "  << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
#define GGO_ASSERT_FLOAT_EQ(v1, v2) if (std::abs(v1 - v2) > 0.001f) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] != "  << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); }
#define	GGO_ASSERT_BTW(v1, v2, v3) if ((v1) < (v2)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] < "  << #v2 << " [=" << (v2) << "] !" << std::endl; GGO_FAIL(); } \
                                     if ((v1) > (v3)) { std::cerr << std::endl << #v1 << " [=" << (v1) << "] > "  << #v3 << " [=" << (v3) << "] !" << std::endl; GGO_FAIL(); }
#else
#define	GGO_TRACE(...)
#define	GGO_ASSERT(zzz)
#define	GGO_ASSERT_PTR(ptr)
#define	GGO_ASSERT_EQ(v1, v2)
#define	GGO_ASSERT_NE(v1, v2)
#define	GGO_ASSERT_LT(v1, v2)
#define	GGO_ASSERT_LE(v1, v2)
#define	GGO_ASSERT_GT(v1, v2)
#define	GGO_ASSERT_GE(v1, v2)
#define GGO_ASSERT_FLOAT_EQ(v1, v2)
#define	GGO_ASSERT_BTW(v1, v2, v3)
#endif
#define	GGO_FAIL() GGO_ASSERT(0)

#endif
