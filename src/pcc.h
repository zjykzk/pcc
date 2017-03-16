#ifndef PCC_H
#define PCC_H
#ifndef PCC_FORCEINLINE
#if defined(_MSC_VER) && defined(NDEBUG)
#define PCC_FORCEINLINE __forceinline
#elif defined(__GNUC__) && __GNUC__ >= 4 && defined(NDEBUG)
#define PCC_FORCEINLINE __attribute__((always_inline))
#else
#define PCC_FORCEINLINE
#endif
#endif // PCC_FORCEINLINE
#endif /* ifndef PCC_H */
