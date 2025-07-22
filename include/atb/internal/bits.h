#pragma once

#define atb_internal_BitMask(N, ...) atb_internal_BitMask_IMPL(N, __VA_ARGS__)
#define atb_internal_BitMask_IMPL(N, ...) atb_internal_BitMask_##N(__VA_ARGS__)

#define atb_internal_BitMask_0() 0x0
#define atb_internal_BitMask_1(a) 0b1 << (a)
#define atb_internal_BitMask_2(a, b) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_1(b))
#define atb_internal_BitMask_3(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_2(__VA_ARGS__))
#define atb_internal_BitMask_4(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_3(__VA_ARGS__))
#define atb_internal_BitMask_5(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_4(__VA_ARGS__))
#define atb_internal_BitMask_6(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_5(__VA_ARGS__))
#define atb_internal_BitMask_7(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_6(__VA_ARGS__))
#define atb_internal_BitMask_8(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_7(__VA_ARGS__))
#define atb_internal_BitMask_9(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_8(__VA_ARGS__))
#define atb_internal_BitMask_10(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_9(__VA_ARGS__))
#define atb_internal_BitMask_11(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_10(__VA_ARGS__))
#define atb_internal_BitMask_12(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_11(__VA_ARGS__))
#define atb_internal_BitMask_13(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_12(__VA_ARGS__))
#define atb_internal_BitMask_14(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_13(__VA_ARGS__))
#define atb_internal_BitMask_15(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_14(__VA_ARGS__))
#define atb_internal_BitMask_16(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_15(__VA_ARGS__))
#define atb_internal_BitMask_17(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_16(__VA_ARGS__))
#define atb_internal_BitMask_18(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_17(__VA_ARGS__))
#define atb_internal_BitMask_19(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_18(__VA_ARGS__))
#define atb_internal_BitMask_20(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_19(__VA_ARGS__))
#define atb_internal_BitMask_21(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_20(__VA_ARGS__))
#define atb_internal_BitMask_22(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_21(__VA_ARGS__))
#define atb_internal_BitMask_23(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_22(__VA_ARGS__))
#define atb_internal_BitMask_24(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_23(__VA_ARGS__))
#define atb_internal_BitMask_25(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_24(__VA_ARGS__))
#define atb_internal_BitMask_26(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_25(__VA_ARGS__))
#define atb_internal_BitMask_27(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_26(__VA_ARGS__))
#define atb_internal_BitMask_28(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_27(__VA_ARGS__))
#define atb_internal_BitMask_29(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_28(__VA_ARGS__))
#define atb_internal_BitMask_30(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_29(__VA_ARGS__))
#define atb_internal_BitMask_31(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_30(__VA_ARGS__))
#define atb_internal_BitMask_32(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_31(__VA_ARGS__))
#define atb_internal_BitMask_33(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_32(__VA_ARGS__))
#define atb_internal_BitMask_34(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_33(__VA_ARGS__))
#define atb_internal_BitMask_35(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_34(__VA_ARGS__))
#define atb_internal_BitMask_36(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_35(__VA_ARGS__))
#define atb_internal_BitMask_37(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_36(__VA_ARGS__))
#define atb_internal_BitMask_38(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_37(__VA_ARGS__))
#define atb_internal_BitMask_39(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_38(__VA_ARGS__))
#define atb_internal_BitMask_40(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_39(__VA_ARGS__))
#define atb_internal_BitMask_41(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_40(__VA_ARGS__))
#define atb_internal_BitMask_42(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_41(__VA_ARGS__))
#define atb_internal_BitMask_43(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_42(__VA_ARGS__))
#define atb_internal_BitMask_44(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_43(__VA_ARGS__))
#define atb_internal_BitMask_45(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_44(__VA_ARGS__))
#define atb_internal_BitMask_46(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_45(__VA_ARGS__))
#define atb_internal_BitMask_47(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_46(__VA_ARGS__))
#define atb_internal_BitMask_48(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_47(__VA_ARGS__))
#define atb_internal_BitMask_49(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_48(__VA_ARGS__))
#define atb_internal_BitMask_50(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_49(__VA_ARGS__))
#define atb_internal_BitMask_51(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_50(__VA_ARGS__))
#define atb_internal_BitMask_52(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_51(__VA_ARGS__))
#define atb_internal_BitMask_53(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_52(__VA_ARGS__))
#define atb_internal_BitMask_54(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_53(__VA_ARGS__))
#define atb_internal_BitMask_55(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_54(__VA_ARGS__))
#define atb_internal_BitMask_56(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_55(__VA_ARGS__))
#define atb_internal_BitMask_57(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_56(__VA_ARGS__))
#define atb_internal_BitMask_58(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_57(__VA_ARGS__))
#define atb_internal_BitMask_59(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_58(__VA_ARGS__))
#define atb_internal_BitMask_60(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_59(__VA_ARGS__))
#define atb_internal_BitMask_61(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_60(__VA_ARGS__))
#define atb_internal_BitMask_62(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_61(__VA_ARGS__))
#define atb_internal_BitMask_63(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_62(__VA_ARGS__))
#define atb_internal_BitMask_64(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_63(__VA_ARGS__))
#define atb_internal_BitMask_65(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_64(__VA_ARGS__))
#define atb_internal_BitMask_66(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_65(__VA_ARGS__))
#define atb_internal_BitMask_67(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_66(__VA_ARGS__))
#define atb_internal_BitMask_68(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_67(__VA_ARGS__))
#define atb_internal_BitMask_69(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_68(__VA_ARGS__))
#define atb_internal_BitMask_70(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_69(__VA_ARGS__))
#define atb_internal_BitMask_71(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_70(__VA_ARGS__))
#define atb_internal_BitMask_72(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_71(__VA_ARGS__))
#define atb_internal_BitMask_73(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_72(__VA_ARGS__))
#define atb_internal_BitMask_74(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_73(__VA_ARGS__))
#define atb_internal_BitMask_75(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_74(__VA_ARGS__))
#define atb_internal_BitMask_76(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_75(__VA_ARGS__))
#define atb_internal_BitMask_77(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_76(__VA_ARGS__))
#define atb_internal_BitMask_78(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_77(__VA_ARGS__))
#define atb_internal_BitMask_79(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_78(__VA_ARGS__))
#define atb_internal_BitMask_80(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_79(__VA_ARGS__))
#define atb_internal_BitMask_81(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_80(__VA_ARGS__))
#define atb_internal_BitMask_82(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_81(__VA_ARGS__))
#define atb_internal_BitMask_83(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_82(__VA_ARGS__))
#define atb_internal_BitMask_84(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_83(__VA_ARGS__))
#define atb_internal_BitMask_85(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_84(__VA_ARGS__))
#define atb_internal_BitMask_86(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_85(__VA_ARGS__))
#define atb_internal_BitMask_87(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_86(__VA_ARGS__))
#define atb_internal_BitMask_88(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_87(__VA_ARGS__))
#define atb_internal_BitMask_89(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_88(__VA_ARGS__))
#define atb_internal_BitMask_90(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_89(__VA_ARGS__))
#define atb_internal_BitMask_91(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_90(__VA_ARGS__))
#define atb_internal_BitMask_92(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_91(__VA_ARGS__))
#define atb_internal_BitMask_93(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_92(__VA_ARGS__))
#define atb_internal_BitMask_94(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_93(__VA_ARGS__))
#define atb_internal_BitMask_95(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_94(__VA_ARGS__))
#define atb_internal_BitMask_96(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_95(__VA_ARGS__))
#define atb_internal_BitMask_97(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_96(__VA_ARGS__))
#define atb_internal_BitMask_98(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_97(__VA_ARGS__))
#define atb_internal_BitMask_99(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_98(__VA_ARGS__))
#define atb_internal_BitMask_100(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_99(__VA_ARGS__))
#define atb_internal_BitMask_101(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_100(__VA_ARGS__))
#define atb_internal_BitMask_102(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_101(__VA_ARGS__))
#define atb_internal_BitMask_103(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_102(__VA_ARGS__))
#define atb_internal_BitMask_104(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_103(__VA_ARGS__))
#define atb_internal_BitMask_105(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_104(__VA_ARGS__))
#define atb_internal_BitMask_106(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_105(__VA_ARGS__))
#define atb_internal_BitMask_107(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_106(__VA_ARGS__))
#define atb_internal_BitMask_108(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_107(__VA_ARGS__))
#define atb_internal_BitMask_109(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_108(__VA_ARGS__))
#define atb_internal_BitMask_110(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_109(__VA_ARGS__))
#define atb_internal_BitMask_111(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_110(__VA_ARGS__))
#define atb_internal_BitMask_112(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_111(__VA_ARGS__))
#define atb_internal_BitMask_113(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_112(__VA_ARGS__))
#define atb_internal_BitMask_114(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_113(__VA_ARGS__))
#define atb_internal_BitMask_115(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_114(__VA_ARGS__))
#define atb_internal_BitMask_116(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_115(__VA_ARGS__))
#define atb_internal_BitMask_117(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_116(__VA_ARGS__))
#define atb_internal_BitMask_118(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_117(__VA_ARGS__))
#define atb_internal_BitMask_119(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_118(__VA_ARGS__))
#define atb_internal_BitMask_120(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_119(__VA_ARGS__))
#define atb_internal_BitMask_121(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_120(__VA_ARGS__))
#define atb_internal_BitMask_122(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_121(__VA_ARGS__))
#define atb_internal_BitMask_123(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_122(__VA_ARGS__))
#define atb_internal_BitMask_124(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_123(__VA_ARGS__))
#define atb_internal_BitMask_125(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_124(__VA_ARGS__))
#define atb_internal_BitMask_126(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_125(__VA_ARGS__))
#define atb_internal_BitMask_127(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_126(__VA_ARGS__))
#define atb_internal_BitMask_128(a, ...) \
  (atb_internal_BitMask_1(a)) | (atb_internal_BitMask_127(__VA_ARGS__))
