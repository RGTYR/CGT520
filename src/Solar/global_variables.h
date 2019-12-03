#ifndef __GLOBAL_VARIABLES_H__
#define __GLOBAL_VARIABLES_H__

static const std::string vertex_shader("gui_demo_vs.glsl");
static const std::string fragment_shader("gui_demo_fs.glsl");
//Texture map for fish
GLuint shader_program = -1;
GLuint texture_id = -1;

static const std::string mesh_name = "Amago0.obj";
static const std::string texture_name = "AmagoT.bmp";
MeshData mesh_data;

// Sphere Mesh
static const std::string sphere_mesh_name = "sphere.obj";
MeshData sphere_mesh_data;

// Cube files and IDs
static const std::string cube_vs("cube_vs.glsl");
static const std::string cube_fs("cube_fs.glsl");
GLuint cube_shader_program = -1;
GLuint cube_vao = -1;
// Texture id for cubemap
static const std::string cube_name = "cubemap";
GLuint cubemap_id = -1;


// Sun files and IDs
Planet sun(10.4, 0, 0.25, 100);
static const std::string sun_vs("sun_vs.glsl");
static const std::string sun_fs("sun_fs.glsl");
GLuint sun_shader_program = -1;
GLuint sun_texture_id = -1;
static const std::string sun_texture_name = "sunmap.bmp";

// Mercury files and IDs
Planet mercury(0.61, 67.0, 0.58, 0.87);
static const std::string mercury_vs("planet_vs.glsl");
static const std::string mercury_fs("planet_fs.glsl");
GLuint mercury_shader_program = -1;
GLuint mercury_texture_id = -1;
static const std::string mercury_texture_name = "mercurymap.bmp";

// venus files and IDs
Planet venus(0.97, 91.0, 2.43, 2.24);
static const std::string venus_vs("planet_vs.glsl");
static const std::string venus_fs("planet_fs.glsl");
GLuint venus_shader_program = -1;
GLuint venus_texture_id = -1;
static const std::string venus_texture_name = "venusmap.bmp";

// earth files and IDs
Planet earth(1, 109.0, 0.01, 3.65);
static const std::string earth_vs("planet_vs.glsl");
static const std::string earth_fs("planet_fs.glsl");
GLuint earth_shader_program = -1;
GLuint earth_texture_id = -1;
static const std::string earth_texture_name = "earthmap.bmp";
static const std::string moon_vs("planet_vs.glsl");
static const std::string moon_fs("planet_fs.glsl");
GLuint moon_shader_program = -1;
GLuint moon_texture_id = -1;
static const std::string moon_texture_name = "moonmap.bmp";

// mars files and IDs
Planet mars(0.73, 133.0, 0.01, 6.86);
static const std::string mars_vs("planet_vs.glsl");
static const std::string mars_fs("planet_fs.glsl");
GLuint mars_shader_program = -1;
GLuint mars_texture_id = -1;
static const std::string mars_texture_name = "marsmap.bmp";

// jupiter files and IDs
Planet jupiter(3.3, 246.0, 0.003, 40.0);
static const std::string jupiter_vs("planet_vs.glsl");
static const std::string jupiter_fs("planet_fs.glsl");
GLuint jupiter_shader_program = -1;
GLuint jupiter_texture_id = -1;
static const std::string jupiter_texture_name = "jupitermap.bmp";

// saturn files and IDs
Planet saturn(0.8, 334.0, 0.004, 10.0, glm::vec3(0.0f, 0.0f, 1.0f));
static const std::string saturn_vs("planet_vs.glsl");
static const std::string saturn_fs("planet_fs.glsl");
GLuint saturn_shader_program = -1;
GLuint saturn_texture_id = -1;
static const std::string saturn_texture_name = "saturnmap.bmp";
static const std::string saturn_mesh_name = "Saturn.obj";
MeshData saturn_mesh_data;

// uranus files and IDs
Planet uranus(2.0, 474.0, 0.006, 306.0);
static const std::string uranus_vs("planet_vs.glsl");
static const std::string uranus_fs("planet_fs.glsl");
GLuint uranus_shader_program = -1;
GLuint uranus_texture_id = -1;
static const std::string uranus_texture_name = "uranusmap.bmp";

// neptune files and IDs
Planet neptune(2.0, 594.0, 0.006, 606.0);
static const std::string neptune_vs("planet_vs.glsl");
static const std::string neptune_fs("planet_fs.glsl");
GLuint neptune_shader_program = -1;
GLuint neptune_texture_id = -1;
static const std::string neptune_texture_name = "neptunemap.bmp";

// asteroid files and IDs
static const std::string asteroid_vs("asteroid_vs.glsl");
static const std::string asteroid_fs("asteroid_fs.glsl");
GLuint asteroid_shader_program = -1;
MeshData deimos_mesh_data;
static const std::string deimos_mesh_name = "deimos.obj";
static const std::string deimos_texture_name = "deimosbump.bmp";
GLuint deimos_texture_id = -1;
int deimos_revolution_radius[1000];
int deimos_revolution_theta[1000];
float deimos_rotation[1000];
MeshData phobos_mesh_data;
static const std::string phobos_mesh_name = "phobos.obj";
static const std::string phobos_texture_name = "phobosbump.bmp";
GLuint phobos_texture_id = -1;
int phobos_revolution_radius[1000];
int phobos_revolution_theta[1000];
float phobos_rotation[1000];

// Particle System
GLuint particles_shader_program = -1;
static const std::string particles_vs("particles_vs.glsl");
static const std::string particles_fs("particles_fs.glsl");
ParticleSystem p_sys(glm::vec3(0.0f), 3000, 10);

float angle = 0.0f;
float scale = 1.0f;
Camera camera;
Camera viewport0;
Camera viewport1(glm::vec3(100.0f, 100.0f, 100.0f));
Camera viewport2(glm::vec3(500.0f, 0.0f, 0.0f));
Camera viewport3(glm::vec3(0.0f, 500.0f, 0.0f));
Camera viewport4(glm::vec3(0.0f, 0.0f, 500.0f));

irrklang::ISoundEngine *SoundEngine = irrklang::createIrrKlangDevice();

std::default_random_engine random_engine;

struct MyTime
{
	float currentTime;
	float lastTime;
	float accountingTime;
	float timeSpeed;
} myTime;

#endif // !__GLOBAL_VARIABLES_H__

