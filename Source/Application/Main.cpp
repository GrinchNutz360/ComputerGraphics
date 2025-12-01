
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;

    std::vector<neu::vec3> points;
    std::vector<neu::vec3> colors;
    std::vector<neu::vec2> texcoords;

    //OPENGL init
    points.push_back(neu::vec3(0.3f, 0.25f, 0.f));
    points.push_back(neu::vec3(0.5f, 0.1f, 0.f));
    points.push_back(neu::vec3(0.f, 0.f, 3.f));
    colors.push_back(neu::vec3(1.f, 0.f, 0.f));
    colors.push_back(neu::vec3(0.f, 1.f, 0.f));
    colors.push_back(neu::vec3(0.f, 0.f, 1.f));
    texcoords.push_back(neu::vec2(0.5f, 0.5f));
    texcoords.push_back(neu::vec2(0.5f, 1.0f));
    texcoords.push_back(neu::vec2(1.0f, 1.0f));

    //OPENGL Initialization
    struct Vertex {
        neu::vec3 position;
        neu::vec3 color;
        neu::vec2 texcoords;
    };
    
    

    //shaders
    auto vs = neu::Resources().Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);
    auto fs = neu::Resources().Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);

    // shader program
    auto program = std::make_shared<neu::Program>();
    program->AttachShader(vs);
    program->AttachShader(fs);
    program->Link();
    program->Use();

	//model
    auto model3d = std::make_shared<neu::Model>();
	model3d->Load("models/cube.obj");
    
    //textures
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/beast.png");
	program->SetUniform("u_texture", 0);


    float rotation = 0.0f;

    glm::vec3 eye{ 0, 0, 5 };

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();
        neu::vec3 color{ 0, 0, 0 };
        float angle = neu::GetEngine().GetTime().GetTime() * 50;        
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;


        // draw
        glm::mat4 model = glm::mat4(1.0f);
        rotation += neu::GetEngine().GetTime().GetDeltaTime() * 50;

        // model matrix
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        program->SetUniform("u_model", model);

        //view matrix
        eye.x += neu::GetEngine().GetInput().GetMouseDelta().x * 0.01f;
        eye.z += neu::GetEngine().GetInput().GetMouseDelta().y * 0.01f;
        glm::mat4 view = glm::lookAt(eye, eye + glm::vec3{ 0,0,-1 }, glm::vec3{ 0,1,0 });
        program->SetUniform("u_view", view);

        //projection matrix
        float aspect = (float)neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
        program->SetUniform("u_projection", projection);

        /*vertexBuffer->Draw();*/

        neu::GetEngine().GetRenderer().Clear();
        model3d->Draw(GL_TRIANGLES);

   
        neu::GetEngine().GetRenderer().Present();

    }

    neu::GetEngine().Shutdown();

    return 0;
}
