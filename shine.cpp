/*
 * $Id: de20237d5cc162405087d61c9732e57b12c6b7d3 $
 * Copyright (C) 2009 Lucid Fusion Labs

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "core/app/app.h"
#include "core/app/shell.h"
#include "core/app/gl/view.h"

namespace LFL {
struct MyApp : public Application {
  unordered_map<string, Shader> shader_map;

  MyApp(int ac, const char* const* av) : Application(ac, av) {}
  virtual ~MyApp();

  void OnWindowInit(Window *W);
  void OnWindowStart(Window *W);
  void OnWindowClosed(Window *W) { delete W; }

  Shader *GetShader(const string &shader_name) { 
    auto shader = shader_map.find(shader_name);
    if (shader == shader_map.end()) return nullptr;
    if (!shader->second.ID) Shader::CreateShaderToy(this, shader_name, FileContents(StrCat(shader_name, ".frag")), &shader->second);
    return &shader->second;
  }
} *app;

inline string   LS  (const char *n) { return app->GetLocalizedString(n); }
inline String16 LS16(const char *n) { return app->GetLocalizedString16(n); }

struct MyView : public View {
  using View::View;

  int Frame(Window *W, unsigned clicks, int flag) {
    float tex[4];
    GraphicsContext gc(W->GD());
    gc.gd->DisableBlend();
    // gc.DrawTexturedBox(W->box, tex, 1);

    W->DrawDialogs();
    W->default_font->Draw(W->gd, StringPrintf("FPS = %.2f", app->focused->fps.FPS()), point(W->gl_w*.85, 0));
    return 0;
  }

  void ChangeShader(const string &shader_name) {
    auto shader = app->GetShader(shader_name);
    // activeshader = shader ? shader : &app->shaders->shader_default;
  }
};

MyApp::~MyApp() {}

void MyApp::OnWindowInit(Window *W) {
  // W->gl_w = app->new_win_width;
  // W->gl_h = app->new_win_height;
  W->caption = app->name;
}

void MyApp::OnWindowStart(Window *W) {
  CHECK(W->gd->have_framebuffer);
  CHECK_EQ(0, W->NewView());
  auto tw = W->ReplaceView(0, make_unique<MyView>(W));
  W->frame_cb = bind(&MyView::Frame, tw, _1, _2, _3);
}

}; // naemspace LFL
using namespace LFL;

extern "C" LFApp *MyAppCreate(int argc, const char* const* argv) {
  FLAGS_enable_video = 1;
  app = make_unique<MyApp>(argc, argv).release();
  app->focused = app->framework->ConstructWindow(app).release();
  app->name = LS("app_name");
  app->window_closed_cb = bind(&MyApp::OnWindowClosed, app, _1);
  app->window_start_cb = bind(&MyApp::OnWindowStart, app, _1);
  app->window_init_cb = bind(&MyApp::OnWindowInit, app, _1);
  app->window_init_cb(app->focused);
  return app;
}

extern "C" int MyAppMain(LFApp*) {
  if (app->Create(__FILE__)) return -1;
  if (app->Init()) return -1;
  app->shader_map.insert(make_pair("warper",   Shader(app)));
  app->shader_map.insert(make_pair("water",    Shader(app)));
  app->shader_map.insert(make_pair("twistery", Shader(app)));
  app->shader_map.insert(make_pair("fire",     Shader(app)));
  app->shader_map.insert(make_pair("waves",    Shader(app)));
  app->shader_map.insert(make_pair("emboss",   Shader(app)));
  app->shader_map.insert(make_pair("stormy",   Shader(app)));
  app->shader_map.insert(make_pair("alien",    Shader(app)));
  app->shader_map.insert(make_pair("fractal",  Shader(app)));
  app->shader_map.insert(make_pair("darkly",   Shader(app)));

  app->StartNewWindow(app->focused);
  return app->Main();
}
