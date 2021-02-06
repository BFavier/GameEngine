#include <GameEngine/GameEngine.hpp>
using namespace GameEngine;

bool base_change()
{
    Referential ref1;
    ref1.name = "ref1";
    ref1.position = Position(1., 1., 0.);
    ref1.orientation = Orientation(Quaternion(-90., {1., 0., 0.}));

    Referential ref2;
    ref2.name = "ref2";
    ref2.position.to(&ref1);
    ref2.position = Position(1., 1., 0., &ref1);
    ref2.orientation.to(&ref1);
    ref2.orientation = Orientation(Quaternion(90., {0., 0., 1.}), &ref1);

    Referential ref3;
    ref3.name = "ref3";
    ref3.position = Position(-1., -1., 0.);
    ref3.orientation = Orientation(Quaternion(180., {0., 0., 1.}));

    Referential ref4;
    ref4.name = "ref4";
    ref4.position.to(&ref3);
    ref4.position = Position(0., -2., 0., &ref3);
    ref4.orientation.to(&ref3);
    ref4.orientation = Orientation(Quaternion(), &ref3);

    Position P(1., 1., -1., &ref2);
    Direction D(1., -1., 1., &ref4);
    Rotation R(Quaternion(75., {1., 1., 0.}), &ref2);
    Orientation O({0., 1., 0.}, {-1., 0., 0.}, {0., 0., 1.}, &ref3);

    //Position
    if (!Vector::equal(Vector(1., 0., -2.), P.in(Referential::absolute)))
    {
        return false;
    }
    if (!Vector::equal(Vector(-2., 1., -2.), P.in(&ref4)))
    {
        return false;
    }
    //Direction
    if (!Vector::equal(Vector(0., 0., -1.), ref2.ux.in(Referential::absolute)))
    {
        return false;
    }
    if (!Vector::equal(Vector(0., 1., 0.), ref4.ux.in(&ref2)))
    {
        return false;
    }
    //Orientation
    if (!Quaternion::equal(Quaternion(-90., {1., 0., 0.})*Quaternion(90., {0., 0., 1.}), ref2.orientation.in(Referential::absolute)))
    {
        return false;
    }
    if (!Quaternion::equal(Quaternion(-180., {0., 0., 1.})*Quaternion(-90., {1., 0., 0.})*Quaternion(90., {0., 0., 1.}), ref2.orientation.in(&ref4)))
    {
        return false;
    }
    if (!Quaternion::equal(Orientation({0., 0., -1.}, {-1., 0., 0.}, {0., 1., 0.}, &ref1), ref2.orientation))
    {
        return false;
    }
    //Rotation
    if (!Quaternion::equal(Quaternion(90., {0., 1., 0.}), Rotation(90., ref2.uz).in(Referential::absolute)))
    {
        return false;
    }
    if (!Quaternion::equal(Quaternion(-90., {0., 1., 0.}), Rotation(90., ref2.uz).in(&ref4)))
    {
        return false;
    }
    //Back and forth conversions
    std::vector<Referential*> refs = {&ref1, &ref2, &ref3, &ref4};
    for (Referential* r1 : refs)
    {
        for (Referential* r2 : refs)
        {
            if (not(Vector::equal(P.in(r1), P.in(r2).in(r1))))
            {
                return false;
            }
            if (not(Vector::equal(D.in(r1), D.in(r2).in(r1))))
            {
                return false;
            }
            if (not(Quaternion::equal(R.in(r1), R.in(r2).in(r1))))
            {
                return false;
            }
            if (not(Quaternion::equal(O.in(r1), O.in(r2).in(r1))))
            {
                return false;
            }
        }
    }
    return true;
}
