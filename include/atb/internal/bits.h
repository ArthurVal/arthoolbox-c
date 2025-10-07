#pragma once

#define ATB_INTERNAL_BitMask(N, ...) ATB_INTERNAL_BitMask_IMPL(N, __VA_ARGS__)
#define ATB_INTERNAL_BitMask_IMPL(N, ...) ATB_INTERNAL_BitMask_##N(__VA_ARGS__)

#define ATB_INTERNAL_BitMask_0() 0x0
#define ATB_INTERNAL_BitMask_1(a) 0b1 << (a)
#define ATB_INTERNAL_BitMask_2(a, b) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_1(b))
#define ATB_INTERNAL_BitMask_3(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_2(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_4(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_3(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_5(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_4(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_6(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_5(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_7(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_6(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_8(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_7(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_9(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_8(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_10(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_9(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_11(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_10(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_12(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_11(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_13(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_12(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_14(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_13(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_15(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_14(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_16(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_15(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_17(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_16(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_18(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_17(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_19(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_18(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_20(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_19(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_21(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_20(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_22(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_21(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_23(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_22(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_24(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_23(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_25(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_24(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_26(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_25(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_27(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_26(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_28(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_27(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_29(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_28(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_30(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_29(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_31(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_30(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_32(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_31(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_33(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_32(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_34(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_33(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_35(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_34(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_36(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_35(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_37(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_36(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_38(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_37(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_39(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_38(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_40(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_39(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_41(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_40(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_42(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_41(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_43(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_42(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_44(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_43(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_45(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_44(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_46(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_45(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_47(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_46(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_48(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_47(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_49(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_48(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_50(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_49(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_51(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_50(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_52(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_51(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_53(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_52(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_54(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_53(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_55(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_54(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_56(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_55(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_57(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_56(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_58(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_57(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_59(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_58(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_60(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_59(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_61(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_60(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_62(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_61(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_63(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_62(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_64(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_63(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_65(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_64(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_66(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_65(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_67(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_66(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_68(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_67(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_69(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_68(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_70(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_69(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_71(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_70(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_72(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_71(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_73(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_72(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_74(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_73(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_75(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_74(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_76(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_75(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_77(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_76(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_78(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_77(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_79(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_78(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_80(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_79(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_81(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_80(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_82(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_81(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_83(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_82(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_84(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_83(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_85(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_84(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_86(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_85(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_87(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_86(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_88(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_87(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_89(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_88(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_90(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_89(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_91(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_90(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_92(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_91(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_93(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_92(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_94(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_93(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_95(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_94(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_96(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_95(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_97(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_96(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_98(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_97(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_99(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_98(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_100(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_99(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_101(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_100(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_102(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_101(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_103(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_102(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_104(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_103(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_105(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_104(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_106(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_105(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_107(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_106(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_108(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_107(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_109(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_108(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_110(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_109(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_111(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_110(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_112(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_111(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_113(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_112(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_114(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_113(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_115(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_114(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_116(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_115(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_117(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_116(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_118(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_117(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_119(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_118(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_120(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_119(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_121(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_120(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_122(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_121(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_123(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_122(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_124(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_123(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_125(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_124(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_126(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_125(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_127(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_126(__VA_ARGS__))
#define ATB_INTERNAL_BitMask_128(a, ...) \
  (ATB_INTERNAL_BitMask_1(a)) | (ATB_INTERNAL_BitMask_127(__VA_ARGS__))
