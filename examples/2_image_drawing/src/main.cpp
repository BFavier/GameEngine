#include <GameEngine/GameEngine.hpp>
using namespace GameEngine;

Model smile_model();
Model flag_model();
void interact(Window& window, StandFPS& stand, Model& model, double dt);

int main(int argc, char* argv[])
{
    //model2
    Model model1 = smile_model();
    Model model2 = flag_model();
    //cameras
    CameraFoV camera1;
    StandFPS stand1(camera1);
    stand1.position = {0., -2., 0.};
    CameraFoV camera2;
    StandFPS stand2(camera2);
    stand2.position = {0., -2., 0.};
    //Windows
    WindowSettings settings;
    settings.title = "window_1";
    settings.width = 500;
    settings.height = 500;
    settings.transparent = true;
    settings.vsync = false;
    //Window window1(settings);
    settings.title = "window_2";
    Window window2(settings);
    //test
    //Image image(2, 2, {{1., 0, 0, 1.}, {0, 1., 0, 1.}, {0, 0, 1., 1.}, {1., 0., 1., 1.}});
    //Image image("images/smile.png");
    //Image test(1, 1);
    //test.clear({1., 0., 0., 0.5});
    //test.download();
    //test.draw(image);
    //test.draw(model2, camera2);
    //test.save("test.png");
    //Miscelianous
    Timer timer;
    while (window2.is_open())// && window1.is_open())
    {
        double dt = timer.dt();
        std::cout << "\r FPS: " << 1/dt << "\t\t\t\t";
        //interact(window1, stand1, model1, dt);
        interact(window2, stand2, model2, dt);
    }
	return 0;
}

void interact(Window& window, StandFPS& stand, Model& model, double dt)
{
    Keyboard& keyboard = window.keyboard;
    Mouse& mouse = window.mouse;
    //moving camera
    if (mouse.hidden())
    {
        Direction dir = {0., 0., 0.};
        double speed = 1.;
        double angle_speed = 5.;
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
    window.clear();//RGBA(0, 0.19, 0.31, 1.));
    window.draw(model, *stand.camera);
    window.update();
}

Model flag_model()
{
    //Mesh
    Mesh* mesh = new Mesh();
    mesh->vertices = {{-0.5, 0., -0.5}, {0.5, 0., -0.5}, {0.5, 0., 0.5}, {-0.5, 0., 0.5}};
    mesh->faces.emplace_back(mesh->vertices[0], mesh->vertices[1], mesh->vertices[3]);
    mesh->faces.emplace_back(mesh->vertices[1], mesh->vertices[2], mesh->vertices[3]);
    mesh->upload();
    //Image
    Image* flag = new Image(2, 2, {{1., 0, 0, 1.}, {0, 1., 0, 1.}, {0, 0, 1., 1.}, {1., 1., 0., 1.}});
    //UVmap
    UVmap* uvmap = new UVmap({{0., 0.}, {1., 0.}, {1., 1.}, {0., 1.}});
    //Material
    Material material;
    material.color = RGBA(0, 0, 0, 0);
    material.two_sided = true;
    //model
    return Model({Part(mesh, material, {Texture(flag, uvmap)})});
}

Model smile_model()
{
    //Mesh
    Mesh* mesh = new Mesh();
    mesh->vertices = {{-0.5, 0., -0.5}, {0.5, 0., -0.5}, {0.5, 0., 0.5}, {-0.5, 0., 0.5}};
    mesh->faces.emplace_back(mesh->vertices[0], mesh->vertices[1], mesh->vertices[3]);
    mesh->faces.emplace_back(mesh->vertices[1], mesh->vertices[2], mesh->vertices[3]);
    mesh->upload();
    //Image
    Image* smile = new Image("./images/smile.png");
    //Image* smile = new Image("./images/iceland.png");
    for (RGBA& pixel : smile->pixels)
    {
        pixel.A = 0.7;
    }
    smile->upload();
    //UVmap
    UVmap* uvmap = new UVmap({{0., 0.}, {1., 0.}, {1., 1.}, {0., 1.}});
    //Material
    Material material;
    material.color = RGBA(0, 0, 0, 0);
    material.two_sided = true;
    //model
    return Model({Part(mesh, material, {Texture(smile, uvmap)})});
}
