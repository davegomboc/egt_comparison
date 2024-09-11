#include "representation/figurine_shapes_type_test.hpp"


#include <sstream>


using namespace representation;


TEST(FigurineShapesType, StreamInsertion) {
    figurine_shapes_type rpnpbpqpkpbpnprp{rook, pawn, knight, pawn, bishop,
     pawn, queen, pawn, king, pawn, bishop, pawn, knight, pawn, rook, pawn};
    std::ostringstream out;
    out << rpnpbpqpkpbpnprp;
    EXPECT_EQ(out.str(), "RPNPBPQPKPBPNPRP");
}


TEST(FigurineShapesType, OneAndTwoShapesEqualityIsAsExpected) {
    figurine_shapes_type k{king};
    figurine_shapes_type kq{king, queen};
    figurine_shapes_type kr{king, rook};
    figurine_shapes_type kb{king, bishop};
    figurine_shapes_type kn{king, knight};
    figurine_shapes_type kp{king, pawn};
    EXPECT_EQ(kq, kq);
    EXPECT_NE(kq, kr);
    EXPECT_NE(kq, kb);
    EXPECT_NE(kq, kn);
    EXPECT_NE(kq, kp);
    EXPECT_NE(kq, k);
    EXPECT_EQ(kr, kr);
    EXPECT_NE(kr, kb);
    EXPECT_NE(kr, kn);
    EXPECT_NE(kr, kp);
    EXPECT_NE(kr, k);
    EXPECT_EQ(kb, kb);
    EXPECT_NE(kb, kn);
    EXPECT_NE(kb, kp);
    EXPECT_NE(kb, k);
    EXPECT_EQ(kn, kn);
    EXPECT_NE(kn, kp);
    EXPECT_NE(kn, k);
    EXPECT_EQ(kp, kp);
    EXPECT_NE(kp, k);
    EXPECT_EQ(k, k);
}


TEST(FigurineShapesType, OneAndTwoShapesOrderingIsAsExpected) {
    figurine_shapes_type k{king};
    figurine_shapes_type kq{king, queen};
    figurine_shapes_type kr{king, rook};
    figurine_shapes_type kb{king, bishop};
    figurine_shapes_type kn{king, knight};
    figurine_shapes_type kp{king, pawn};
    EXPECT_LE(kq, kq);
    EXPECT_GE(kq, kq);
    EXPECT_LT(kq, kr);
    EXPECT_GT(kr, kq);
    EXPECT_LT(kq, kb);
    EXPECT_GT(kb, kq);
    EXPECT_LT(kq, kn);
    EXPECT_GT(kn, kq);
    EXPECT_LT(kq, kp);
    EXPECT_GT(kp, kq);
    EXPECT_LT(kq, k);
    EXPECT_GT(k, kq);
    EXPECT_LE(kr, kr);
    EXPECT_GE(kr, kr);
    EXPECT_LT(kr, kb);
    EXPECT_GT(kb, kr);
    EXPECT_LT(kr, kn);
    EXPECT_GT(kn, kr);
    EXPECT_LT(kr, kp);
    EXPECT_GT(kp, kr);
    EXPECT_LT(kr, k);
    EXPECT_GT(k, kr);
    EXPECT_LE(kb, kb);
    EXPECT_GE(kb, kb);
    EXPECT_LT(kb, kn);
    EXPECT_GT(kn, kb);
    EXPECT_LT(kb, kp);
    EXPECT_GT(kp, kb);
    EXPECT_LT(kb, k);
    EXPECT_GT(k, kb);
    EXPECT_LE(kn, kn);
    EXPECT_GE(kn, kn);
    EXPECT_LT(kn, kp);
    EXPECT_GT(kp, kn);
    EXPECT_LT(kn, k);
    EXPECT_GT(k, kn);
    EXPECT_LE(kp, kp);
    EXPECT_GE(kp, kp);
    EXPECT_LT(kp, k);
    EXPECT_GT(k, kp);
    EXPECT_LE(k, k);
    EXPECT_GE(k, k);
}
