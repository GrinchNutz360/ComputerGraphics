
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
    std::vector<neu::vec3> points{ { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 } }; 
    std::vector<neu::vec3> colors{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    std::vector<neu::vec2> texcoord{ {0,0}, {0.5f,1.0f}, {1,1} };


    GLuint vbo[3];    
    glGenBuffers(3, vbo);

    //vertex buffer (position)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * points.size(),points.data(), GL_STATIC_DRAW);

    //vertex buffer (color)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);

    //vertex buffer (texcoord)
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(neu::vec2) * texcoord.size(), texcoord.data(), GL_STATIC_DRAW);

    //vertex array
    GLuint vao;
    glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

    //position
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //color
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //texcoord
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

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

    
    //textures
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/beast.png");


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
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size());

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
