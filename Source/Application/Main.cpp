
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene


    SDL_Event e;
    bool quit = false;

    //OPENGL Initialization
    struct Vertex {
        neu::vec3 position;
        neu::vec3 color;
        neu::vec2 texcoords;
    };
    
    //material
    auto material = neu::Resources().Get<neu::Material>("materials/spot.mat");

    // shader program
    auto program = neu::Resources().Get<neu::Program>("Shaders/basic_lit.prong");
    material->program->Use();

	//model
    auto model3d = std::make_shared<neu::Model>();
	model3d->Load("models/cow.obj");
    
    //textures
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/cow.png");
	texture->Bind();


    // lights
    material->program->SetUniform("u_ambient_light", glm::vec3{ 0.5f });
    neu::Transform light{ { 2,4,3 } };
	glm::vec3 lightColor{1};

    glm::vec3 eye{ 0, 0, 5 };
    neu::Transform camera{ { 0, 0, 5 } };
    neu::Transform transform{ { 0, 0, 0 } };

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            ImGui_ImplSDL3_ProcessEvent(&e);
        }

        // update
        neu::GetEngine().Update();
        float dt = neu::GetEngine().GetTime().GetDeltaTime();
        neu::vec3 color{ 0, 0, 0 };
        transform.rotation.y += 90 * dt;
        float angle = neu::GetEngine().GetTime().GetTime() * 50;        
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;


        // draw

        //start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        //set ImGui frame
        ImGui::Begin("Editor");
        ImGui::DragFloat3("Position", glm::value_ptr(light.position), 0.1f);
        ImGui::ColorEdit3("Color", glm::value_ptr(lightColor));
        ImGui::DragFloat("Shininess", &material->shininess, 0.1f);
        ImGui::DragFloat2("tiling", glm::value_ptr(material->tiling), 0.0);
        ImGui::Text("Hello World");
        ImGui::Text("Press 'Esc' to quit.");
        ImGui::End();

        glm::mat4 model = glm::mat4(1.0f);
        
        

        // model matrix
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        material->program->SetUniform("u_model", model);

        //view matrix
        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.y += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.y -= speed * dt;
        
        glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
        material->program->SetUniform("u_view", view);

        material->program->SetUniform("u_light.position", (glm::vec3)(view* glm::vec4(light.position, 1)));
        light.position.x = neu::math::sin(neu::GetEngine().GetTime().GetTime() * 2) * 30;
        material->program->SetUniform("u_light.color", glm::vec3{ 0,25,0 });

        //projection matrix
        float aspect = (float)neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
        material->program->SetUniform("u_projection", projection);

        /*vertexBuffer->Draw();*/

        neu::GetEngine().GetRenderer().Clear();
        
        material->Bind();
        model3d->Draw(GL_TRIANGLES);

        // draw ImGui
        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

   
        neu::GetEngine().GetRenderer().Present();

    }

    neu::GetEngine().Shutdown();

    return 0;
}
