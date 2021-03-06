//------------------------------------------------------------------------------
//  animMgrTest.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "UnitTest++/src/UnitTest++.h"
#include "Anim/AnimTypes.h"
#include "Anim/private/animMgr.h"

using namespace Oryol;
using namespace _priv;

TEST(AnimLibraryTest) {

    // setup
    AnimSetup setup;
    setup.MaxNumLibs = 4;
    setup.ClipPoolCapacity = 16;
    setup.CurvePoolCapacity = 128;
    setup.KeyPoolCapacity = 1024;
    setup.ResourceLabelStackCapacity = 16;
    setup.ResourceRegistryCapacity = 24;
    animMgr mgr;
    mgr.setup(setup);
    CHECK(mgr.isValid);
    CHECK(mgr.resContainer.IsValid());
    CHECK(mgr.resContainer.registry.IsValid());
    CHECK(mgr.libPool.IsValid());
    CHECK(mgr.clipPool.Capacity() == 16);
    CHECK(mgr.curvePool.Capacity() == 128);
    CHECK(mgr.keys.Size() == 1024);
    CHECK(mgr.keys.Offset() == 0);
    CHECK(mgr.numKeys == 0);
    CHECK(mgr.valuePool != nullptr);

    AnimLibrarySetup libSetup;
    libSetup.Locator = "human";
    libSetup.CurveLayout = {
        AnimCurveFormat::Float2,
        AnimCurveFormat::Float3,
        AnimCurveFormat::Float4
    };
    libSetup.Clips = {
        { "clip1", 10, 0.04f, 
            {
                { false, 1.0f, 2.0f, 3.0f, 4.0f },
                { false, 5.0f, 6.0f, 7.0f, 8.0f },
                { true,  9.0f, 10.0f, 11.0f, 12.0f },
            }
        },
        { "clip2", 20, 0.04f, 
            {
                { true,  4.0f, 3.0f, 2.0f, 1.0f },
                { false, 8.0f, 7.0f, 6.0f, 5.0f },
                { true,  12.0f, 11.0f, 10.0f, 9.0f }
            }
        }
    };

    ResourceLabel l1 = mgr.resContainer.PushLabel();
    Id lib1 = mgr.createLibrary(libSetup);
    mgr.resContainer.PopLabel();
    CHECK(lib1.IsValid());
    CHECK(mgr.lookupLibrary(lib1) != nullptr);
    CHECK(mgr.libPool.QueryPoolInfo().NumUsedSlots == 1);
    CHECK(mgr.clipPool.Size() == 2);
    CHECK(mgr.curvePool.Size() == 6);
    CHECK(mgr.numKeys == 110);
    const AnimLibrary* lib1Ptr = mgr.lookupLibrary(lib1);
    CHECK(lib1Ptr->Locator.Location() == "human");
    CHECK(lib1Ptr->SampleStride == 9);
    CHECK(lib1Ptr->Clips.Size() == 2);
    CHECK(lib1Ptr->Clips[0].Name == "clip1");
    CHECK(lib1Ptr->Clips[0].Length == 10);
    CHECK(lib1Ptr->Clips[0].KeyStride == 5);
    CHECK(lib1Ptr->Clips[0].Keys.Size() == 50);
    CHECK(lib1Ptr->Clips[0].Keys.Offset() == 0);
    CHECK(lib1Ptr->Clips[0].Curves.Size() == 3);
    CHECK(lib1Ptr->Clips[0].Curves.Offset() == 0);
    CHECK(lib1Ptr->Clips[0].Curves[0].Format == AnimCurveFormat::Float2);
    CHECK(lib1Ptr->Clips[0].Curves[0].KeyStride == 2);
    CHECK(!lib1Ptr->Clips[0].Curves[0].Static);
    CHECK(lib1Ptr->Clips[0].Curves[0].KeyIndex == 0);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[0].StaticValue[0], 1.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[0].StaticValue[1], 2.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[0].StaticValue[2], 3.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[0].StaticValue[3], 4.0f, 0.001f);
    CHECK(lib1Ptr->Clips[0].Curves[1].Format == AnimCurveFormat::Float3);
    CHECK(lib1Ptr->Clips[0].Curves[1].KeyStride == 3);
    CHECK(!lib1Ptr->Clips[0].Curves[1].Static);
    CHECK(lib1Ptr->Clips[0].Curves[1].KeyIndex == 2);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[1].StaticValue[0], 5.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[1].StaticValue[1], 6.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[1].StaticValue[2], 7.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[1].StaticValue[3], 8.0f, 0.001f);
    CHECK(lib1Ptr->Clips[0].Curves[2].Format == AnimCurveFormat::Float4);
    CHECK(lib1Ptr->Clips[0].Curves[2].KeyStride == 0);
    CHECK(lib1Ptr->Clips[0].Curves[2].Static);
    CHECK(lib1Ptr->Clips[0].Curves[2].KeyIndex == InvalidIndex);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[2].StaticValue[0], 9.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[2].StaticValue[1], 10.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[2].StaticValue[2], 11.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[0].Curves[2].StaticValue[3], 12.0f, 0.001f);
    CHECK(lib1Ptr->Clips[1].Name == "clip2");
    CHECK(lib1Ptr->Clips[1].Length == 20);
    CHECK(lib1Ptr->Clips[1].KeyStride == 3);
    CHECK(lib1Ptr->Clips[1].Keys.Size() == 60);
    CHECK(lib1Ptr->Clips[1].Keys.Offset() == 50);
    CHECK(lib1Ptr->Clips[1].Curves.Size() == 3);
    CHECK(lib1Ptr->Clips[1].Curves.Offset() == 3);
    CHECK(lib1Ptr->Clips[1].Curves[0].Format == AnimCurveFormat::Float2);
    CHECK(lib1Ptr->Clips[1].Curves[0].KeyStride == 0);
    CHECK(lib1Ptr->Clips[1].Curves[0].Static);
    CHECK(lib1Ptr->Clips[1].Curves[0].KeyIndex == InvalidIndex);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[0].StaticValue[0], 4.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[0].StaticValue[1], 3.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[0].StaticValue[2], 2.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[0].StaticValue[3], 1.0f, 0.001f);
    CHECK(lib1Ptr->Clips[1].Curves[1].Format == AnimCurveFormat::Float3);
    CHECK(lib1Ptr->Clips[1].Curves[1].KeyStride == 3);
    CHECK(!lib1Ptr->Clips[1].Curves[1].Static);
    CHECK(lib1Ptr->Clips[1].Curves[1].KeyIndex == 0);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[1].StaticValue[0], 8.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[1].StaticValue[1], 7.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[1].StaticValue[2], 6.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[1].StaticValue[3], 5.0f, 0.001f);
    CHECK(lib1Ptr->Clips[1].Curves[2].Format == AnimCurveFormat::Float4);
    CHECK(lib1Ptr->Clips[1].Curves[2].KeyStride == 0);
    CHECK(lib1Ptr->Clips[1].Curves[2].Static);
    CHECK(lib1Ptr->Clips[1].Curves[2].KeyIndex == InvalidIndex);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[2].StaticValue[0], 12.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[2].StaticValue[1], 11.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[2].StaticValue[2], 10.0f, 0.001f);
    CHECK_CLOSE(lib1Ptr->Clips[1].Curves[2].StaticValue[3], 9.0f, 0.001f);
    
    libSetup.Locator = "Bla";
    Id lib2 = mgr.createLibrary(libSetup);
    CHECK(lib2.IsValid());
    CHECK(mgr.lookupLibrary(lib2) != nullptr);
    CHECK(mgr.libPool.QueryPoolInfo().NumUsedSlots == 2);
    CHECK(mgr.clipPool.Size() == 4);
    CHECK(mgr.curvePool.Size() == 12);
    CHECK(mgr.numKeys == 220);
    const AnimLibrary* lib2Ptr = mgr.lookupLibrary(lib2);
    CHECK(lib2Ptr->Locator.Location() == "Bla");
    CHECK(lib2Ptr->SampleStride == 9);
    CHECK(lib2Ptr->Clips.Size() == 2);
    CHECK(lib2Ptr->Clips[0].Name == "clip1");
    CHECK(lib2Ptr->Clips[0].Length == 10);
    CHECK(lib2Ptr->Clips[0].KeyStride == 5);
    CHECK(lib2Ptr->Clips[0].Keys.Size() == 50);
    CHECK(lib2Ptr->Clips[0].Keys.Offset() == 110);
    CHECK(lib2Ptr->Clips[0].Curves.Size() == 3);
    CHECK(lib2Ptr->Clips[0].Curves.Offset() == 6);
    CHECK(lib2Ptr->Clips[0].Curves[0].Format == AnimCurveFormat::Float2);
    CHECK(lib2Ptr->Clips[0].Curves[0].KeyStride == 2);
    CHECK(!lib2Ptr->Clips[0].Curves[0].Static);
    CHECK(lib2Ptr->Clips[0].Curves[0].KeyIndex == 0);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[0].StaticValue[0], 1.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[0].StaticValue[1], 2.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[0].StaticValue[2], 3.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[0].StaticValue[3], 4.0f, 0.001f);
    CHECK(lib2Ptr->Clips[0].Curves[1].Format == AnimCurveFormat::Float3);
    CHECK(lib2Ptr->Clips[0].Curves[1].KeyStride == 3);
    CHECK(!lib2Ptr->Clips[0].Curves[1].Static);
    CHECK(lib2Ptr->Clips[0].Curves[1].KeyIndex == 2);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[1].StaticValue[0], 5.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[1].StaticValue[1], 6.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[1].StaticValue[2], 7.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[1].StaticValue[3], 8.0f, 0.001f);
    CHECK(lib2Ptr->Clips[0].Curves[2].Format == AnimCurveFormat::Float4);
    CHECK(lib2Ptr->Clips[0].Curves[2].KeyStride == 0);
    CHECK(lib2Ptr->Clips[0].Curves[2].Static);
    CHECK(lib2Ptr->Clips[0].Curves[2].KeyIndex == InvalidIndex);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[2].StaticValue[0], 9.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[2].StaticValue[1], 10.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[2].StaticValue[2], 11.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[0].Curves[2].StaticValue[3], 12.0f, 0.001f);
    CHECK(lib2Ptr->Clips[1].Name == "clip2");
    CHECK(lib2Ptr->Clips[1].Length == 20);
    CHECK(lib2Ptr->Clips[1].KeyStride == 3);
    CHECK(lib2Ptr->Clips[1].Keys.Size() == 60);
    CHECK(lib2Ptr->Clips[1].Keys.Offset() == 160);
    CHECK(lib2Ptr->Clips[1].Curves.Size() == 3);
    CHECK(lib2Ptr->Clips[1].Curves.Offset() == 9);
    CHECK(lib2Ptr->Clips[1].Curves[0].Format == AnimCurveFormat::Float2);
    CHECK(lib2Ptr->Clips[1].Curves[0].KeyStride == 0);
    CHECK(lib2Ptr->Clips[1].Curves[0].Static);
    CHECK(lib2Ptr->Clips[1].Curves[0].KeyIndex == InvalidIndex);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[0].StaticValue[0], 4.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[0].StaticValue[1], 3.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[0].StaticValue[2], 2.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[0].StaticValue[3], 1.0f, 0.001f);
    CHECK(lib2Ptr->Clips[1].Curves[1].Format == AnimCurveFormat::Float3);
    CHECK(lib2Ptr->Clips[1].Curves[1].KeyStride == 3);
    CHECK(!lib2Ptr->Clips[1].Curves[1].Static);
    CHECK(lib2Ptr->Clips[1].Curves[1].KeyIndex == 0);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[1].StaticValue[0], 8.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[1].StaticValue[1], 7.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[1].StaticValue[2], 6.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[1].StaticValue[3], 5.0f, 0.001f);
    CHECK(lib2Ptr->Clips[1].Curves[2].Format == AnimCurveFormat::Float4);
    CHECK(lib2Ptr->Clips[1].Curves[2].KeyStride == 0);
    CHECK(lib2Ptr->Clips[1].Curves[2].Static);
    CHECK(lib2Ptr->Clips[1].Curves[2].KeyIndex == InvalidIndex);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[2].StaticValue[0], 12.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[2].StaticValue[1], 11.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[2].StaticValue[2], 10.0f, 0.001f);
    CHECK_CLOSE(lib2Ptr->Clips[1].Curves[2].StaticValue[3], 9.0f, 0.001f);
    mgr.destroy(l1);
    CHECK(mgr.libPool.QueryPoolInfo().NumUsedSlots == 1);
    CHECK(mgr.clipPool.Size() == 2);
    CHECK(mgr.curvePool.Size() == 6);
    CHECK(mgr.numKeys == 110);

    mgr.discard();
    CHECK(!mgr.isValid);
    CHECK(mgr.clipPool.Size() == 0);
    CHECK(mgr.curvePool.Size() == 0);
    CHECK(mgr.numKeys == 0);
}
