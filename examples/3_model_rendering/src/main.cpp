#include <GameEngine/GameEngine.hpp>
using namespace GameEngine;

int main(int argc, char* argv[])
{
    /*
    Mesh mesh;
    mesh.vertices = {{-0.5, 0., -0.5}, {0.5, 0., -0.5}, {0.5, 0., 0.5}, {-0.5, 0., 0.5}};
    mesh.faces.emplace_back(mesh.vertices[0], mesh.vertices[1], mesh.vertices[3]);
    mesh.faces.emplace_back(mesh.vertices[1], mesh.vertices[2], mesh.vertices[3]);
    mesh.upload();
    Image image("./images/smile.png");
    Image flag(2, 2, {{255, 0, 0, 150}, {0, 255, 0, 150}, {0, 0, 255, 150}, {255, 0, 255, 150}}, false);
    UVmap uvmap = {{0., 0.}, {1., 0.}, {1., 1.}, {0., 1.}};
    Part part(mesh);
    part.material.color = RGBA(100, 0, 0, 255);
    part.material.two_sided = true;
    part.textures.emplace_back(image, uvmap);
    part.textures.emplace_back(flag, uvmap);
    part.upload();
    Model model({part});
    model.position = {0., 1., 0.};
    */
    Timer timer;
    CameraFoV camera;
    StandFPS stand(camera);
    stand.position = {0., -2., 0.};
    Window window("MyGame", 800, 600);
    Keyboard& keyboard = window.keyboard;
    Mouse& mouse = window.mouse;
    mouse.hide(true);
    //Model model("models/backpack/backpack.obj");
    //Model model("models/nanosuit/nanosuit.obj");
    Model model("models/tree/tree.fbx");
    Model model2(model);
    for (Part& part : model2.parts)
    {
        part.textures.clear();
        part.material.color.A = 100;
        part.upload();
    }
    model.position += {0., 1., 0.};
    while (window.is_open())
    {
        //moving camera
        if (mouse.hidden())
        {
            Direction dir = {0., 0., 0.};
            double speed = 1.;
            double angle_speed = 5.;
            double dt = timer.dt();
            if (keyboard.key("Q").down)
            {
                //dir -= camera.sideward;
                dir -= stand.sideward;
            }
            if (keyboard.key("D").down)
            {
                //dir += camera.sideward;
                dir += stand.sideward;
            }
            if (keyboard.key("Z").down)
            {
                //dir += camera.forward;
                dir += stand.forward;
            }
            if (keyboard.key("S").down)
            {
                //dir -= camera.forward;
                dir -= stand.forward;
            }
            if (dir.norm() > 0.)
            {
                //camera.position += dir.normed()*speed*dt;
                stand.position += dir.normed()*speed*dt;
            }
            double dx = mouse.dx();
            if (dx != 0.)
            {
                //camera.orientation += Rotation(-dx*angle_speed*dt, camera.upward);
                stand.yaw(-dx*angle_speed*dt);
            }
            double dy = mouse.dy();
            if (dy != 0.)
            {
                //camera.orientation -= Rotation(dy*angle_speed*dt, camera.sideward);
                stand.pitch(-dy*angle_speed*dt);
            }
        }
        if (keyboard.key("ESCAPE").down)
        {
            window.close();
        }
        if (keyboard.key("ENTER").down && keyboard.key("ENTER").changed)
        {
            mouse.hide(!mouse.hidden());
        }
        //drawing
        window.clear(RGBA(0, 50, 80, 255));
        window.draw(model2, camera);
        window.draw(model, camera);
        window.update();
    }
	return 0;
}
