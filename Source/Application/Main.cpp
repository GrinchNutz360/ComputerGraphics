
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
        neu::vec2 textcoords;
    };

    std::vector<Vertex> vertices{
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ -0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    };

    std::vector<GLuint> indices{
       0, 1, 2,
       0, 2, 3
    };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // index buffer
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* indices.size(), indices.data(), GL_STATIC_DRAW);

    //vertex array
    GLuint vao;
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textcoords));

    auto vs = neu::Resources().Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);
    auto fs = neu::Resources().Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);

 //   //vertex shader
 //   std::string vs_source;
 //   neu::file::ReadTextFile("shaders/basic.vert", vs_source);
 //   const char* vs_cstr = vs_source.c_str();

 //   GLuint vs;
 //   vs = glCreateShader(GL_VERTEX_SHADER);
 //   glShaderSource(vs, 1, &vs_cstr, NULL);

 //   glCompileShader(vs);
 //   
 //   glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
 //   if (!success)
 //   {
 //       std::string infoLog(512, '\0');  // pre-allocate space
 //       GLsizei length;
 //       glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
 //       infoLog.resize(length);

 //       LOG_WARNING("Shader compilation failed: {}", infoLog);
 //   }

 //   //fragment shader 
	//std::string fs_source;
	//neu::file::ReadTextFile("shaders/basic.frag", fs_source);
	//const char* fs_cstr = fs_source.c_str();

	//GLuint fs;
	//fs = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fs, 1, &fs_cstr, NULL);
	//glCompileShader(fs);

 //   glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
 //   if (!success)
 //   {
 //       std::string infoLog(512, '\0');  // pre-allocate space
 //       GLsizei length;
 //       glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
 //       infoLog.resize(length);

 //       LOG_WARNING("Shader compilation failed: {}", infoLog);
 //   }

    // shader program
    auto program = std::make_shared<neu::Program>();
    program->AttachShader(vs);
    program->AttachShader(fs);
    program->Link();
    program->Use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    
    //textures
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/beast.png");
	program->SetUniform("u_texture", 0);

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();
        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;
        program->SetUniform("u_time", neu::GetEngine().GetTime().GetTime());
		
        
        /*float angle = neu::GetEngine().GetTime().GetTime() * 90.0f;
        float scale = neu::math::Remap(-1.0f, 1.0f, 0.3f, 1.5f, neu::math::sin(neu::GetEngine().GetTime().GetTime()));
		
        neu::vec2 mouse = neu::GetEngine().GetInput().GetMousePosition();
        neu::vec2 position;

		position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mouse.x);
		position.y = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetHeight(), 1.0f, -1.0f, mouse.y);*/

        

        // draw
        neu::GetEngine().GetRenderer().Clear();
        program->SetUniform("u_model", model);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        

        /*glLoadIdentity();

        glPushMatrix();
		glScalef(scale, scale, 0);
		glRotatef(angle, 0, 0, 1);
		glTranslatef(position.x, position.y, 0);
     
        glBegin(GL_TRIANGLES);

        for (int i = 0; i < points.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }
		glPopMatrix();
            
        glEnd();*/

   
        neu::GetEngine().GetRenderer().Present();

    }

    neu::GetEngine().Shutdown();

    return 0;
}
