#include <cassert>
#include <unordered_map>
#include <SDL2/SDL_image.h>
#include "utils_colour.hpp"
#include "utils_types.hpp"
#include "backend_handler_sdl.hpp"


namespace // anonymous
{
  // holds pointers to loaded textures to avoid reloading textures more than once.
  using namespace std;
  using namespace Engine::BackEnd;

  static
  std::unordered_map< string, shared_ptr< SDL2::Texture > > textures;

  // this anonymous (restricted) namespace contains a
  // singleton class that inits SDL and its subsystems

  class SDLWrapper
  {
  public:
    ~SDLWrapper();

    static SDLWrapper * instance();

    bool initVideo();
    bool initEvents();
    bool initInput();

    void quitVideo();
    void quitEvents();
    void quitInput();

  protected:
    SDLWrapper();
    void init();
    void quit();
  };

  inline SDLWrapper * SDLWrapper::instance()
  {
    static SDLWrapper * s_instance = nullptr;

    if(nullptr == s_instance)
    {
      s_instance = new SDLWrapper();
    }
    return s_instance;
  }

  inline SDLWrapper::SDLWrapper()
  {
    this->init();
  }

  inline SDLWrapper::~SDLWrapper()
  {
    this->quit();
  }

  inline void SDLWrapper::init()
  {
    int initSdl = SDL_Init(SDL_INIT_NOPARACHUTE);
    if (initSdl < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
    }
    assert(initSdl == 0);

    int flags = IMG_INIT_PNG;
    int initImg = IMG_Init(flags);
    if (initImg < 0)
    {
      SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL Image: %s", SDL_GetError());
    }
    assert(initImg == flags);
  }

  inline void SDLWrapper::quit()
  {
    IMG_Quit();
    SDL_Quit();
  }

  inline bool initSubsystem(const Engine::Flags flag)
  {
    bool wasInit = SDL_WasInit(flag);
    if(!wasInit)
    {
      return SDL_InitSubSystem(flag);
    }
    return wasInit;
  }

  inline bool SDLWrapper::initVideo()
  {
    return initSubsystem(SDL_INIT_VIDEO);
  }

  inline bool SDLWrapper::initEvents()
  {
    return initSubsystem(SDL_INIT_EVENTS | SDL_INIT_TIMER);
  }

  inline bool SDLWrapper::initInput()
  {
    return initSubsystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
  }

  inline void quitSubsystem(const Engine::Flags flag)
  {
    bool wasInit = SDL_WasInit(flag);
    if(wasInit)
    {
      SDL_QuitSubSystem(flag);
    }
  }

  inline void SDLWrapper::quitVideo()
  {
    quitSubsystem(SDL_INIT_VIDEO);
  }

  inline void SDLWrapper::quitEvents()
  {
    quitSubsystem(SDL_INIT_EVENTS | SDL_INIT_TIMER);
  }

  inline void SDLWrapper::quitInput()
  {
    quitSubsystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
  }

} // end namespace anonymous


namespace Engine
{
using namespace std;
using namespace Utils;

namespace BackEnd
{

namespace SDL2
{

  // converts the float values from Colour into SDL RGB8
  void colour8RGBA(u8 & r, u8 & g, u8 & b, u8 & a, const Engine::Colour & c)
  {
    r = 0;
    g = 0;
    b = 0;
    a = 255;

    switch (c.kind)
    {
      case Engine::RGB:
      {
        r = (static_cast<u8>(c.rgb.r * 255) % 256);
        g = (static_cast<u8>(c.rgb.g * 255) % 256);
        b = (static_cast<u8>(c.rgb.b * 255) % 256);
        break;
      }
      case Engine::RGBA:
      {
        r = (static_cast<u8>(c.rgba.r * 255) % 256);
        g = (static_cast<u8>(c.rgba.g * 255) % 256);
        b = (static_cast<u8>(c.rgba.b * 255) % 256);
        a = (static_cast<u8>(c.rgba.a * 255) % 256);
        break;
      }
      case Engine::HSL:
      {
        ColourRGB tmp;
        Hsl2Rgb(c.hsl, tmp);
        r = (static_cast<u8>(tmp.r * 255) % 256);
        g = (static_cast<u8>(tmp.g * 255) % 256);
        b = (static_cast<u8>(tmp.b * 255) % 256);
        break;
      }
      case Engine::HSLA:
      {
        ColourRGB tmp;
        Hsl2Rgb({c.hsla.h, c.hsla.s, c.hsla.l }, tmp);
        r = (static_cast<u8>(tmp.r * 255) % 256);
        g = (static_cast<u8>(tmp.g * 255) % 256);
        b = (static_cast<u8>(tmp.b * 255) % 256);
        a = (static_cast<u8>(c.hsla.a * 255) % 256);
      }
      break;
      case Engine::HSV:
      {
        ColourRGB tmp;
        Hsv2Rgb(c.hsv, tmp);
        r = (static_cast<u8>(tmp.r * 255) % 256);
        g = (static_cast<u8>(tmp.g * 255) % 256);
        b = (static_cast<u8>(tmp.b * 255) % 256);
        break;
      }
      case Engine::HSVA:
      {
        ColourRGB tmp;
        Hsl2Rgb({c.hsva.h, c.hsva.s, c.hsva.v }, tmp);
        r = (static_cast<u8>(tmp.r * 255) % 256);
        g = (static_cast<u8>(tmp.g * 255) % 256);
        b = (static_cast<u8>(tmp.b * 255) % 256);
        a = (static_cast<u8>(c.hsva.a * 255) % 256);
      }
      break;
      case Engine::CMYK:
      {
        ColourRGB tmp;
        Cmyk2Rgb(c.cmyk, tmp);
        r = (static_cast<u8>(tmp.r * 255) % 256);
        g = (static_cast<u8>(tmp.g * 255) % 256);
        b = (static_cast<u8>(tmp.b * 255) % 256);
        break;
      }
      case Engine::HEX:
      {
        //  TODO
        break;
      }
    }
  }

  // here we define classes related to SDL Handler;

  // wrapper around window and renderer

  // only one instance of the window is allowed
  Dimension2 Renderer::_resolution = { 640.0, 480.0};
  SDL_Window * Renderer::_window = nullptr;
  SDL_Renderer * Renderer::_renderer = nullptr;

  Renderer::Renderer()
  {
    if(SDL_WasInit(SDL_INIT_VIDEO) && nullptr == this->_window &&  nullptr == this->_renderer)
    {
      if (  SDL_CreateWindowAndRenderer(
                static_cast<int>(_resolution.width),
                static_cast<int>(_resolution.height),
                SDL_WINDOW_OPENGL,
                &this->_window,
                &this->_renderer) )
      {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
      }
    }
    assert(this->_window && this->_renderer);
  }

  Renderer::~Renderer()
  {
    if(SDL_WasInit(SDL_INIT_VIDEO) && nullptr == this->_window &&  nullptr == this->_renderer)
    {
      SDL_DestroyRenderer(this->_renderer);
      this->_renderer = nullptr;
      SDL_DestroyWindow(this->_window);
      this->_window = nullptr;
    }
  }

  // wrapper around texture
  Texture::Texture(const string & filepath, SDL_Renderer * renderer)
  {
    SDL_Surface * surface = IMG_Load(filepath.c_str());
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
          assert(false);
    }
    else
    {
      this->_buffer = SDL_CreateTextureFromSurface(renderer, surface);
      if (!this->_buffer) {
          SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
      }
      _rect.topLeft = { 0.0f, 0.0f };
      _rect.size = {static_cast<SpaceDim>(surface->w),
                    static_cast<SpaceDim>(surface->h)};
      SDL_FreeSurface(surface);
    }
    assert(this->_buffer);
  }

  shared_ptr< Texture > Texture::createTexture(
      const string & filepath,
      SDL_Renderer * renderer)
  {
    shared_ptr< Texture > texPtr(nullptr);

    auto sdlTexIt = textures.find(filepath);
    if(sdlTexIt != textures.end())
    {
      texPtr = sdlTexIt->second;
    }
    else
    {

      texPtr.reset(new Texture(filepath, renderer));
      textures[filepath] = texPtr;
    }
    return texPtr;
  }

  Texture::~Texture()
  {
    // if this texture was shared it might have been destroye already!
    if(nullptr != this->_buffer)
    {
      SDL_DestroyTexture(this->_buffer);
      this->_buffer = nullptr;
    }
  }

  // context used by viewport and texture

  Handler::Handler()
      :_view(new Renderer())
      ,_image(nullptr)
  {}

  // graphic functions
  bool initGraphicSystem(Flags flags)
  {
    return SDLWrapper::instance()->initVideo();
  }

  void quitGraphicSystem()
  {
    SDLWrapper::instance()->quitVideo();
  }

} // end namespace SDL2


} // end namespace BackEnd

} // end namespace Engine
