#include "tut/tut.hpp"

#include "stg/blowfish.h"

namespace
{

bool equalCtx(const BLOWFISH_CTX & a, const BLOWFISH_CTX & b)
{
    for (size_t i = 0; i < sizeof(a.P); ++i)
        if (a.P[i] != b.P[i]) {
            //printf("Failed for P at %d: 0%x != 0%x\n", i, a.P[i], b.P[i]);
            return false;
        }
    for (size_t i = 0; i < 4; ++i)
        for (size_t j = 0; j < 256; ++j)
            if (a.S[i][j] != b.S[i][j]) {
                //printf("Failed for S at %d, %d: 0%x != 0%x\n", i, j, a.S[i][j], b.S[i][j]);

                return false;
            }
    return true;
}

bool equalString(const char* a, const char* b, size_t length)
{
    bool res = true;
    for (size_t i = 0; i < length; ++i)
        if (a[i] != b[i]) {
            //printf("Failed at pos %d: %hhu != %hhu\n", i, a[i], b[i]);
            res = false;
        }
    return res;
}

const BLOWFISH_CTX testCtx =
{
    {
        0x84ddf841, 0xbbdc17f,
        0x3dd821af, 0x66fdf0c,
        0x426a2fff, 0x30223f2c,
        0x8325f1a3, 0xf5c99ab9,
        0x417ce6da, 0xba4c9637,
        0xd226c646, 0xe50ea3b5,
        0x3860f82c, 0x1565ecc8,
        0xf4461621, 0x68058dd4,
        0x217da3bb, 0xd6cd3ddc,
    },
    {
        {
            0x72f9dc5c, 0xf701a2e4, 0xf30e3ec3, 0x2f1c9683,
            0x985b9801, 0xfb28c4f5, 0xf6f5c777, 0xfa7b5b24,
            0xd761c693, 0xf53e1484, 0x8622da13, 0xb240d1b4,
            0xd1bc58f8, 0x43f462ef, 0x6a4e97a3, 0xb0619beb,
            0xff07b462, 0xd390c3b9, 0xe91f2840, 0x869566ae,
            0xfebd249c, 0x567002c1, 0x342e0e8e, 0x31b01687,
            0xd7e3a5bd, 0xa933de2e, 0xb4ef05f2, 0x33e4919f,
            0x724e8b3e, 0xc6b9d44f, 0x849b1674, 0x90758946,
            0x295986fa, 0xa2e11855, 0xf2099a4e, 0x42ed46f9,
            0x597cbb6d, 0xc3691952, 0x408f873e, 0xdee0d6cf,
            0x6570a45d, 0xc6a24b81, 0x2d3a1c24, 0x4a2edda4,
            0x443dcfa5, 0x5971a9a5, 0xfd2258c0, 0x72c36b37,
            0xf8696ce, 0xf2e84f7, 0xc543bf6d, 0x352818c5,
            0xdc01f156, 0xc67bfce5, 0xdadf0cbf, 0x6194b6a9,
            0xdb3cc22f, 0xc7873bcb, 0x9b9e622c, 0xa7869810,
            0xcb1ba91, 0x137b1bf5, 0x3e71252b, 0x5229f33e,
            0x30ded88f, 0x5763ef1f, 0xee7f01b8, 0xdac73dee,
            0x2f4d1efb, 0x5f518b97, 0xe7c7d61e, 0x9d5ca3f3,
            0xa196c020, 0x74634a7b, 0xdd1d0042, 0x454268aa,
            0x5c610ac8, 0x277e8d2c, 0x4a8b4ee8, 0xf959f5a8,
            0xc0c63a7e, 0xea9f2ed7, 0xaab4a699, 0x6f872358,
            0x3927189a, 0x23ae56ac, 0xcfa5b09c, 0x9751e84,
            0x7ec68c4b, 0x99f9502b, 0x886e764, 0x957c89a4,
            0x48c5ddb0, 0x9f4d5b7e, 0xa84d5c52, 0xf2ae7bde,
            0x75e6ddaf, 0xc2b45798, 0x5f846059, 0x5bf85e2a,
            0x5f6c4382, 0xb621465a, 0x5bf2977c, 0x2c1eb17a,
            0xbf3e5b18, 0x5f0bdcb7, 0xd6195c47, 0x2c61ec83,
            0x2315c2da, 0x95195c86, 0x4c4f9855, 0xc5e3787d,
            0xe5014918, 0x714dd230, 0xd2abd926, 0x9229a99e,
            0x8306695a, 0x66f7bb54, 0x544affdc, 0x84aa830c,
            0xad8c8db6, 0x48b29b2, 0xcf33d204, 0xc78ac51d,
            0xacb18e60, 0x49b516ac, 0x614e2887, 0xe39e1a7c,
            0xdb1f9652, 0x8c8378a3, 0x8cbe2070, 0x773fe7f,
            0x899e1922, 0x12dd7b8b, 0xccf8198d, 0xcc48faff,
            0x30ca9cab, 0x9ebd7129, 0xf254b601, 0x8ac97b38,
            0x504804d1, 0x3568fbbe, 0x9a40bc99, 0x9c23761b,
            0x28e26904, 0xe396e583, 0x2f2c4053, 0xa45caf19,
            0xb7996794, 0x4c825669, 0x3f07f001, 0xf24a084b,
            0x884888fd, 0xa7fc71f4, 0x21abeff4, 0x719ecba6,
            0x90104162, 0x50c2b5f1, 0x86135070, 0xc9523fc,
            0x9c219eca, 0xc12ef7f0, 0x6b3f6b8c, 0x288ee9ef,
            0xe722f758, 0x229c0bac, 0x3d934d09, 0x4a5cf131,
            0x14189fc9, 0x10c8aa2e, 0x74e9fe96, 0xcfc110d4,
            0x35202bfb, 0x90bc66f1, 0x5d4c6616, 0x498927ea,
            0x581934cc, 0x43e712a, 0x5f095b3c, 0xea0e1406,
            0xa4921c29, 0xe8a306f0, 0xfec369ef, 0x992921c1,
            0xcdc5abb6, 0x3ebd83fa, 0x8a7f16e9, 0xfaf73bb3,
            0x3e8181, 0x34fc47c9, 0xdb92d725, 0x8b40795,
            0x14273fa0, 0xd704e4f8, 0xd3435939, 0x3450c0a8,
            0xce721627, 0x10d4a466, 0x7bc1a724, 0x5efc63b,
            0x153ae5ef, 0xa0aa6945, 0xe5a3b8c9, 0xf0bec5ba,
            0xde02619e, 0xd155e989, 0xf5e3a551, 0x951d9d74,
            0x41544875, 0x6aa455e8, 0xda28a584, 0x72d17a11,
            0x417c066c, 0xe6f82546, 0xbd258cd0, 0x8a3979b4,
            0xe896e0d9, 0x76871921, 0x709842f1, 0x5dd3756b,
            0x2ebe8dfb, 0x81ab8b0d, 0x4c9d635, 0x3fd75cf2,
            0x37876ef8, 0x1c5a806f, 0x96de1b30, 0x2a8e971a,
            0x89ac843a, 0x96c23371, 0x95ee2343, 0xffb1614f,
            0x1116003c, 0x25b45917, 0xad0783c2, 0x4662ce95,
            0xfc751ade, 0xa13e75ae, 0xc75010c3, 0x9170a859,
            0x86ca62d6, 0x3acb094f, 0x87349773, 0x2765ba9b,
            0xaaebd756, 0x7b3d6693, 0x92193a80, 0x765f4eb8,
            0x42f88c21, 0x27ff9f9b, 0x9db88664, 0xe302e6d9,
            0x2e8fb36e, 0xfc58b9bc, 0x3a649297, 0x76876ce1
        },
        {
            0x8e3ea6e4, 0x1dd37803, 0xffd051c0, 0xec395f57,
            0x2eae16f7, 0x821505b4, 0x7ab81f78, 0xf2c88c63,
            0xa7655b7c, 0xc436644, 0x18e67d7c, 0xa5c8e283,
            0x6e44a66e, 0xe7547024, 0x280ce7d0, 0x8ec06da2,
            0x93683af4, 0x5e39165c, 0x77721743, 0x7b9d837e,
            0x32fed2ef, 0x9726079b, 0x98a579a, 0xc51b8dc5,
            0xedd45d61, 0xbf70e897, 0x5c8187f8, 0x83909156,
            0x469c6dc2, 0xf6fb34f5, 0xf14e0646, 0x5e988165,
            0x3dea4f92, 0x51b2d53c, 0xb543c4e, 0xae450075,
            0xf9aaffee, 0x4bee49e7, 0x60ffe4f6, 0x395318e3,
            0x8acb2b46, 0x6df56797, 0x52c9d67, 0x1ffd024a,
            0x2499d763, 0x951c8244, 0x560ceb0, 0x5d0490a2,
            0x7208da46, 0xfe40f9d7, 0xd2b96165, 0x696f3072,
            0x2ba20225, 0x8758c6b4, 0x71f59b60, 0xd84d9bd3,
            0x8bc05ab, 0xa09fa33, 0xd14769cf, 0x238c630a,
            0x5635a041, 0x9af1f0bc, 0xa347515, 0xa90ac427,
            0xdbcbc880, 0x9434490c, 0xcb041e0f, 0x4101f58,
            0xcb057a30, 0x721e4a62, 0x8d59b73, 0x76d1c883,
            0x724d675, 0x57833e73, 0xf65e3cc7, 0x4025c165,
            0x61caa9fa, 0xecf3ffad, 0xc9ccba59, 0xbf1e78c0,
            0xe7c9f808, 0x5f3ae0ad, 0xe338467, 0x5baf328,
            0x77ea9442, 0x232d063c, 0x44c1f91f, 0xcfaaa06,
            0xcd95c059, 0xf92f3f43, 0xd105984a, 0x4ea4eac2,
            0xdb17fb06, 0xed567697, 0x896b421d, 0x40c3b884,
            0xf3258ace, 0xbdb93689, 0xb75abba5, 0xc69f182c,
            0x5798ae31, 0xff3ba2e7, 0x1ef383b4, 0xc689850c,
            0xa0e850e, 0x7478b55d, 0xf0cd99e7, 0x78128b44,
            0x99139a31, 0x191a4f69, 0x4dcb9de1, 0x80d9512c,
            0x6dd3001a, 0xcb6f465e, 0xaa760561, 0x19cc07b3,
            0x383c9dc4, 0x620f19d8, 0xbc621d05, 0x888061f0,
            0x549fb42, 0xd21c5ac2, 0x4e37b133, 0xda38e932,
            0x66ac40ae, 0x855de0db, 0x29c32890, 0xfc79f537,
            0xea0bc017, 0xb30fd396, 0x9f6e65d0, 0x1fbd8e56,
            0xa6a79b7b, 0x63871bcf, 0x977361fe, 0x572fc22e,
            0xcac4d530, 0xee9631a1, 0xd66ca305, 0xd8648a0b,
            0xa0fa4f44, 0x1b49d414, 0xb9b89101, 0xbb3dc53b,
            0x9267697d, 0x998ee205, 0xca1a4a0b, 0x16e0c5d3,
            0xb099860c, 0x3ec1b23b, 0x5d11b6a5, 0x78daf576,
            0xd58d0926, 0x7e77c453, 0x42a2daf5, 0x6465314c,
            0xa25d7243, 0x33eb1745, 0x9d662159, 0x61e86fcb,
            0xedca8ac0, 0xc4afdc42, 0x866e5492, 0x9e72fe28,
            0x9e5b7096, 0xb3121ab3, 0x9eafbc0d, 0xb03b48,
            0x3a29c9d, 0x517d9ea7, 0xd12dd262, 0x8965c881,
            0x3f1575bf, 0x811afeb2, 0xb8699338, 0xa49b7870,
            0x8b72b9aa, 0xa56dbc23, 0xbdc66113, 0xc80baa85,
            0x2f6e2fe2, 0xe2f32eaa, 0x4942b340, 0x3782424d,
            0xc7f3fd93, 0x29c6dbac, 0x181c1bc1, 0x7db26d5e,
            0x49ecc6f6, 0x7f600a7f, 0xf73b236a, 0x264d7fd1,
            0x68bac57f, 0xd276c9e, 0x1784d004, 0xb98b5fc7,
            0xd03f81ca, 0xd44d46a1, 0xcfbea6d1, 0x5c06d9d0,
            0xbec1bad1, 0xa3bacc94, 0xaacc518c, 0xf8e20ef9,
            0x8749c85a, 0xe4f865cd, 0x819fdb8a, 0x3c9924f6,
            0xcb96ac1, 0x3a2bc069, 0x34b28508, 0xdb02aaf2,
            0x231dccd0, 0xe088dc6a, 0x4d54c84, 0x9096e29f,
            0x92db735a, 0x44ebca10, 0x8bbe55f2, 0x17287085,
            0xf782f3b5, 0xc98a9a13, 0x117c0991, 0x69d34986,
            0xa53303c9, 0x786c1b84, 0x5c174525, 0x4887510b,
            0xaaf68a8e, 0xff4cba8f, 0xb6b66cc7, 0x1d5de8ea,
            0xb98783a8, 0x82525dd6, 0x41200306, 0xaf354785,
            0xb3479dc5, 0x508c2a19, 0x61a3104e, 0x5688a9f2,
            0xca4bee32, 0x45df7f55, 0x2d2caedd, 0xdfdffc59,
            0x911454c1, 0xd9dd2d80, 0x4e328041, 0x582ebac0,
            0xbda7aab2, 0x10c52f26, 0x472690d0, 0x258304cf,
            0x54e2e210, 0xd4482bf8, 0x22086561, 0x98407334
        },
        {
            0x900aa5b7, 0x4e649e1f, 0xc9d635c9, 0x69ffda97,
            0xdce34ccd, 0xf08ff29b, 0xee9360d3, 0xd9c59300,
            0x7190589d, 0x69d75cc1, 0x9e3c3dc1, 0x45541e71,
            0x6f1bccc2, 0x46f39b64, 0xc2622672, 0xfacf3cca,
            0x867a7957, 0xd14995f, 0xfaa6011d, 0xaeaa8c37,
            0x3d911958, 0x5f672f3a, 0x80ffb57, 0x7c27f22d,
            0x295fd1f, 0x55f3a836, 0x55476bd5, 0x1d717b6b,
            0x99b01c2, 0xf7a4359a, 0xfbf0d88f, 0x90f0a4fd,
            0xdd1bed5d, 0x395e2267, 0x84473b44, 0x216d1700,
            0xd91466b6, 0xf6723abc, 0xb58917f7, 0xe4055634,
            0x5e5c94a9, 0x371a7adc, 0xa9444660, 0x9cd864c7,
            0x942160f8, 0xfa57e367, 0x3ac900aa, 0xfed09d6c,
            0xab1e8ce6, 0x1c3440b2, 0x2921f7ef, 0xcbc8aad9,
            0xaee21a52, 0xb65bac8c, 0x4e24db59, 0xa0afe727,
            0xd6a6a2ce, 0xcda6dc7c, 0xde7b8be, 0xac98135d,
            0x61ebab33, 0xd6c91e9a, 0x42afb446, 0x5ffdebdd,
            0x22d4df79, 0xd19e8fe7, 0xbc6b9029, 0x597ff071,
            0xa4f6f069, 0xa41ed335, 0xa933f4c8, 0x3a4b5456,
            0xca3df8d6, 0x91915bbd, 0x7c656515, 0x6ae7e70d,
            0x517118c, 0x2a9565e0, 0x3046470a, 0x4d0b8c6,
            0x788182c, 0x3bf3c684, 0x1e7a37f5, 0x45507790,
            0xb9ab569e, 0x1d3927fe, 0x2aac516d, 0x8eb519e0,
            0xc7563e93, 0x8572cdd1, 0xcfe62ded, 0x78eeb7d6,
            0x152032bb, 0x5f480f75, 0xa1d06607, 0x51da64b0,
            0xbc666eb1, 0x34ea7eec, 0x6ccff8b1, 0xd952ba,
            0x32d16b6, 0xcbfe265e, 0x9daab452, 0xb12f4349,
            0xceac2beb, 0x4b9787f9, 0x85719171, 0xf0b9c027,
            0x7feeaa15, 0x990aebcc, 0x767bf1a9, 0xcd56802d,
            0xe718ab1e, 0x661ef622, 0x3e217d8f, 0x8604b297,
            0x712d7706, 0xff2a8240, 0xe149f2cd, 0x49c36be2,
            0x3ac40571, 0xa041a748, 0xaa1580cb, 0x7a9a3e62,
            0x9def968d, 0x908f3347, 0x40dddf27, 0xfd078597,
            0x30b9d025, 0xd40d138, 0xde35af82, 0x5718e93,
            0x529cd4ac, 0x41632ca0, 0x169c0ff3, 0xbac5b27a,
            0x771b0fa7, 0xdcdf8bd, 0x4faf140e, 0x5aec8617,
            0x24597031, 0x6407c4a6, 0x70fc5d31, 0x9085ff31,
            0x8b1397d8, 0x99633b58, 0x91d5b56f, 0xfb451a1a,
            0xe379e1fc, 0xe6ba2f3f, 0x462cb5c1, 0x1dd06ff4,
            0x2cf546ba, 0x70800782, 0xdc27b1eb, 0x7965ad5d,
            0x39ffac7a, 0x862ffce9, 0x569e938e, 0xd42c7cc3,
            0x614f729b, 0x842ba8d5, 0xc4c56289, 0x77df9d86,
            0xf51eb421, 0xec140c11, 0x2b891d79, 0x58b98151,
            0x49955e, 0x6d2ae4cb, 0xdb9bd16f, 0x816ed489,
            0x5187b66a, 0xe2428c32, 0x340ca67c, 0x6002c78f,
            0xe7479dfd, 0xcc924e44, 0xc73ec342, 0xf17cfd8d,
            0x8942a89b, 0x75d34569, 0xac22e545, 0x8375e876,
            0x8395884a, 0x586698a6, 0x3f3d8f12, 0xdc5943d3,
            0x66d1996b, 0xd06a5896, 0x6a8afde7, 0xb4d9accb,
            0x17289e6a, 0xff9b64ef, 0x47c3dab, 0xfa2e58,
            0x26790980, 0x5163e6f6, 0x65e17492, 0x62e55e79,
            0x8af1b5a0, 0x2c72e6ba, 0x47da4a0a, 0x8a20bab4,
            0xf3bcbd52, 0x16d2a483, 0xe5877bcf, 0x3cade4ba,
            0xf65721a2, 0xb5815461, 0xb3fb53b5, 0xf74508db,
            0x5c5c1af, 0x3b53868, 0xd9c6825d, 0x4a55bb36,
            0x785ce186, 0xd5732f60, 0xd94fadb0, 0x1676d0cf,
            0x1090d57f, 0x7bd7a227, 0xac20c78c, 0x2c68e495,
            0xcd6478d2, 0xc7f4641c, 0x5ed5d389, 0x2cdc09ca,
            0xff6d94d2, 0x47d4921c, 0xc3da6fc4, 0x6231f4c6,
            0x28643ecd, 0x9367f303, 0x2319a3a2, 0x2f86fd8e,
            0x7f13a567, 0x785e871a, 0xf88564cf, 0x27203f51,
            0x7f7fe69a, 0xf0bbc1f4, 0x6ac32b0, 0xfa87113a,
            0x1533dab5, 0x9e168c28, 0x621b244d, 0xc26cdca8,
            0xe2b12ff1, 0xb1fb41d, 0x74abc0bf, 0x6faf66ed,
            0x7f96eef7, 0x8dd96699, 0xe0afd88c, 0x5666ae1e
        },
        {
            0x429f8c4d, 0x7a6f751a, 0xe1f9f104, 0x8f3352a2,
            0xee11c98f, 0xd3e00903, 0x834b9614, 0x1ca1cd5f,
            0xc5da95e5, 0x48aa87f9, 0xf3e28f3a, 0xb3d2f1cd,
            0x11a99c75, 0x6fc626d4, 0x75c5dbf4, 0x9a055ab0,
            0x878bd7d0, 0x797cb4fe, 0xb56af6f2, 0xd8dad771,
            0xd79d651f, 0xecc885c2, 0xe36ba61b, 0x3af46b8b,
            0xd4c4dec7, 0x1ef69803, 0x24c094f2, 0x4796f5ca,
            0xd0b4f1a4, 0x35b71a50, 0xc93bda59, 0x88744ee4,
            0xe435da7e, 0xc2add479, 0xf89eba5b, 0xeb7b3be7,
            0x25abda16, 0x31bc2a9, 0xd46c3697, 0xbdab88a5,
            0xaf765372, 0x776715c1, 0x91f39946, 0x5a46fca5,
            0xb7345689, 0xe5ccffc6, 0x83094686, 0xd29b145,
            0x947c5b14, 0xee6a48b9, 0xf80bcf7e, 0x80364978,
            0x28785457, 0x7f01f829, 0x2480cb12, 0x1c4e853d,
            0x9c5be553, 0xbc234906, 0xf48c6576, 0x20d7163e,
            0xd13fb6aa, 0x7911e4f2, 0x74a4c545, 0xcd37db61,
            0x923daabd, 0x3671e21f, 0x474b7b1, 0xd6c142a8,
            0xfd74fe19, 0x967d3c94, 0x76507cd3, 0x1860dcdf,
            0x40511fa8, 0x19feec2b, 0xa624cb9, 0x3125d93b,
            0xc4bed3ba, 0xaa215b42, 0xa44f7c85, 0x63065ae5,
            0xde95bfe, 0x6b6e92e8, 0xf3e516d1, 0x8bc680c8,
            0x5fdc0cad, 0x6e7281d4, 0x1a4a5a56, 0x5edb95a3,
            0xb91324c1, 0xbbfbce4c, 0x9e90028c, 0x4825c43d,
            0xe5f599a4, 0xcacc8db0, 0xf1fd746b, 0x2d7baad9,
            0xcc3148c4, 0xffefb22f, 0x9ec43aee, 0x7556ebfc,
            0x7ce98693, 0x19eb04dd, 0xc5c3d0e2, 0x5d0c1bea,
            0x801fe923, 0xcb47549d, 0xada4ca94, 0x953a3ff6,
            0x92d8bc7e, 0x1a8b21fa, 0x7e7d47ca, 0x345de887,
            0x668fc1d5, 0xb191b107, 0x29f30bbd, 0x669cdc3a,
            0x907fad0c, 0xa8432d5d, 0x8125844d, 0x6f8ee014,
            0x96fe6488, 0x6b812b55, 0x96e27fb5, 0xb68e0b09,
            0x6d28e784, 0x370fff6c, 0x65c77431, 0xa6ba0fe6,
            0x85ede90f, 0x6ebc916e, 0xd2927076, 0x8f7f7b17,
            0x690c30f5, 0x88c8404e, 0x76b88278, 0x27eb857b,
            0xfd20c71d, 0x53b63cc4, 0x93fb6326, 0xca3a6e63,
            0x879e4bc7, 0x7e75fa05, 0x2dc60e82, 0xc4b2b95,
            0xe6e17a43, 0x6e6c53a7, 0xe6baa075, 0x7b50f168,
            0xf95e02a8, 0x3422dd2d, 0x4eb38261, 0xd17615c8,
            0x238c1d99, 0x24def9f8, 0x250a45d1, 0xaeb97793,
            0xd6bf65e6, 0xf4080ae1, 0x90814d9e, 0x2d9d8436,
            0x2d742949, 0xa9afb64c, 0xdd4d18f8, 0x8855711e,
            0x856bf7df, 0xbeb28a6b, 0xff5d789b, 0xd1e24c20,
            0x92581004, 0xf9ee25e5, 0xdd445cff, 0xb5bc99cd,
            0x2e90fb9d, 0x86e95fa6, 0x3ce3fc89, 0x2ccff4c9,
            0x42b6541c, 0xf2cdb401, 0x76b6cd96, 0x5cbc0f31,
            0x5d988c8e, 0x5535eb69, 0x1354397b, 0x5f105b42,
            0x70359c13, 0x8053d6a9, 0x43ee0e25, 0x7279a17b,
            0x23b44313, 0x99dbf710, 0x3671f4f1, 0x212fb10a,
            0xd61b85ef, 0x5370eec8, 0xb03ac259, 0x53068883,
            0x10325203, 0x2550b3b5, 0x3afa6580, 0x5945266f,
            0xf8f21594, 0x1f2feef2, 0xcaebbffc, 0xcc55f93a,
            0x21f277bf, 0x4e1c1b9c, 0xdd74f5a4, 0x34e84677,
            0xb85d1834, 0xf9fdccaa, 0x5f13cc3, 0x8d1a00d9,
            0xd3b7f623, 0x2d8886bf, 0x1505627e, 0x476ea29b,
            0x7758fa96, 0xc59274f8, 0x5d6cd56, 0xf892f3ce,
            0xc26a482c, 0xe00f252c, 0x38c981c3, 0x223869ac,
            0xaa8bc73f, 0xe8ca1111, 0xed51fdd6, 0xeb0724d4,
            0x61b05512, 0x290876c2, 0x50b855d1, 0x20cf2267,
            0x894a6a06, 0xd87f0ab4, 0x5961c4f5, 0xdd7781a,
            0x8f898f63, 0xe5bcfae8, 0x2ab999c3, 0x6d25c711,
            0x11b0621c, 0x7a097422, 0xa7f23f1, 0x9fbfb450,
            0xd5e5019b, 0xbe3427a9, 0x70967c5c, 0x18f65981,
            0x2c9bde6b, 0xfe6f03c4, 0xc754ef9a, 0x637d1ff5,
            0x9749ef7a, 0xae0c1da6, 0x4e05c89c, 0x87e4150a
        }
    }
};

const unsigned char testString[] = { 68, 100, 2, 115, 6, 54, 226, 228 };

} // namespace anonymous

namespace tut
{
    struct crypto_data {
    };

    typedef test_group<crypto_data> tg;
    tg crypto_test_group("Crypto tests group");

    typedef tg::object testobject;

    template<>
    template<>
    void testobject::test<1>()
    {
        set_test_name("Check context creation");

        BLOWFISH_CTX ctx;
        InitContext("pr7Hhen", 7, &ctx);
        ensure("ctx == testCtx", equalCtx(ctx, testCtx));
    }

    template<>
    template<>
    void testobject::test<2>()
    {
        set_test_name("Check encryption");

        BLOWFISH_CTX ctx;
        InitContext("pr7Hhen", 7, &ctx);
        uint32_t a = 0x12345678;
        uint32_t b = 0x87654321;
        Blowfish_Encrypt(&ctx, &a, &b);

        ensure_equals("a == 0xd3988cd", a, 0xd3988cd);
        ensure_equals("b == 0x7996c6d6", b, 0x7996c6d6);
    }

    template<>
    template<>
    void testobject::test<3>()
    {
        set_test_name("Check decryption");

        BLOWFISH_CTX ctx;
        InitContext("pr7Hhen", 7, &ctx);
        uint32_t a = 0xd3988cd;
        uint32_t b = 0x7996c6d6;
        Blowfish_Decrypt(&ctx, &a, &b);

        ensure_equals("a == 0x12345678", a, 0x12345678);
        ensure_equals("b == 0x87654321", b, 0x87654321);
    }

    template<>
    template<>
    void testobject::test<4>()
    {
        set_test_name("Check block encryption");

        BLOWFISH_CTX ctx;
        InitContext("pr7Hhen", 7, &ctx);
        uint32_t block[2] = {0x12345678, 0x87654321};
        EncryptBlock(&block, &block, &ctx);

        ensure_equals("block[0] == 0xd3988cd", block[0], 0xd3988cd);
        ensure_equals("block[1] == 0x7996c6d6", block[1], 0x7996c6d6);
    }

    template<>
    template<>
    void testobject::test<5>()
    {
        set_test_name("Check block decryption");

        BLOWFISH_CTX ctx;
        InitContext("pr7Hhen", 7, &ctx);
        uint32_t block[2] = {0xd3988cd, 0x7996c6d6};
        DecryptBlock(&block, &block, &ctx);

        ensure_equals("block[0] == 0x12345678", block[0], 0x12345678);
        ensure_equals("block[1] == 0x87654321", block[1], 0x87654321);
    }

    template<>
    template<>
    void testobject::test<6>()
    {
        set_test_name("Check string encryption");

        BLOWFISH_CTX ctx;
        InitContext("pr7Hhen", 7, &ctx);
        char res[8];
        EncryptString(res, "testtest", 8, &ctx);

        ensure("EncryptString(\"testtest\") == testString", equalString(res, (char *)testString, 8));
    }

    template<>
    template<>
    void testobject::test<7>()
    {
        set_test_name("Check long string encryption");

        BLOWFISH_CTX ctx;
        InitContext("pr7Hhen", 7, &ctx);
        std::string source("abcdefghijklmnopqrstuvwxyz 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        char longTest[source.length() + 8];
        EncryptString(longTest, source.c_str(), source.length() + 1, &ctx);
        DecryptString(longTest, longTest, sizeof(longTest), &ctx);

        ensure_equals("DecryptString(EncryptString(longTest)) == longTest", source, std::string(longTest));
    }

    template<>
    template<>
    void testobject::test<8>()
    {
        set_test_name("Check old string encryption");

        BLOWFISH_CTX ctx;
        InitContext("123456", 7, &ctx);
        const unsigned char source[] = {0xe9, 0xfe, 0xcb, 0xc5, 0xad, 0x3e, 0x87, 0x39,
                                        0x3d, 0xd5, 0xf4, 0xed, 0xb0, 0x15, 0xe6, 0xcb,
                                        0x3d, 0xd5, 0xf4, 0xed, 0xb0, 0x15, 0xe6, 0xcb,
                                        0x3d, 0xd5, 0xf4, 0xed, 0xb0, 0x15, 0xe6, 0xcb};
        char res[32];
        DecryptString(res, source, 32, &ctx);

        ensure_equals("DecryptString(...) == 'admin'", std::string(res), "admin");
    }

    template<>
    template<>
    void testobject::test<9>()
    {
        set_test_name("Check new string encryption");

        BLOWFISH_CTX ctx;
        InitContext("123456", 7, &ctx);
        const unsigned char source[] = {0xe9, 0xfe, 0xcb, 0xc5, 0xad, 0x3e, 0x87, 0x39,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        char res[32];
        DecryptString(res, source, 32, &ctx);

        ensure_equals("DecryptString(...) == 'admin'", std::string(res), "admin");
    }

}
