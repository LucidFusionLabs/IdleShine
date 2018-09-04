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
  using Application::Application;
  void OnWindowStart(Window *W);
} *app;

inline string   LS  (const char *n) { return app->GetLocalizedString(n); }
inline String16 LS16(const char *n) { return app->GetLocalizedString16(n); }

struct MyView : public View {
  using View::View;
  unordered_map<string, Shader> shader_map;
  Shader *activeshader = &app->shaders->shader_default;

  MyView(Window *R, const Box &B=Box()) : View(R, B) {
    shader_map.insert(make_pair("warper",   Shader(app)));
    shader_map.insert(make_pair("water",    Shader(app)));
    shader_map.insert(make_pair("twistery", Shader(app)));
    shader_map.insert(make_pair("fire",     Shader(app)));
    shader_map.insert(make_pair("waves",    Shader(app)));
    shader_map.insert(make_pair("emboss",   Shader(app)));
    shader_map.insert(make_pair("stormy",   Shader(app)));
    shader_map.insert(make_pair("alien",    Shader(app)));
    shader_map.insert(make_pair("fractal",  Shader(app)));
    shader_map.insert(make_pair("darkly",   Shader(app)));
  }
    
  int Frame(Window *W, unsigned clicks, int flag) {
    Box draw_box = W->Box();
    GraphicsContext gc(W->GD());
    gc.gd->ClearColor(Color::red);
    gc.gd->DisableBlend();

    float scale = activeshader->scale;
    glShadertoyShader(gc.gd, activeshader);
    activeshader->SetUniform1i("iChannelFlip", 1);
    activeshader->SetUniform2f("iChannelScroll", 0, 0);
    activeshader->SetUniform4f("iTargetBox", draw_box.x, draw_box.y,
                               XY_or_Y(scale, draw_box.w), XY_or_Y(scale, draw_box.h));
    GraphicsContext::DrawTexturedBox1(gc.gd, draw_box);
    gc.gd->UseShader(0);

    W->DrawDialogs();
    W->default_font->Draw(W->gd, StringPrintf("FPS = %.2f", app->focused->fps.FPS()), point(0, 0));
    return 0;
  }

  void ChangeShader(const string &shader_name) {
    auto shader = GetShader(shader_name);
    activeshader = shader ? shader : &app->shaders->shader_default;
  }

  Shader *GetShader(const string &shader_name) { 
    auto shader = shader_map.find(shader_name);
    if (shader == shader_map.end()) return nullptr;
    if (!shader->second.ID) Shader::CreateShaderToy(app, shader_name, app->FileContents(StrCat(shader_name, ".frag")), &shader->second);
    return &shader->second;
  }
};

void MyApp::OnWindowStart(Window *W) {
  CHECK_EQ(0, W->NewView());
  auto V = W->ReplaceView(0, make_unique<MyView>(W));
  W->frame_cb = bind(&MyView::Frame, V, _1, _2, _3);
  V->ChangeShader("waves");
}

}; // naemspace LFL
using namespace LFL;

extern "C" LFApp *MyAppCreate(int argc, const char* const* argv) {
  FLAGS_enable_video = 1;
  Application::LoadDefaultSettings(StringPairVec{
    StringPair("send_crash_reports", "1"),
    StringPair("write_log_file",     "0")
  });
  if (true || atoi(Application::GetSetting("write_log_file"))) {
    FLAGS_logfile = "\x01";
    FLAGS_loglevel = 7;
  }
  app = make_unique<MyApp>(argc, argv).release();
  app->focused = app->framework->ConstructWindow(app).release();
  app->name = LS("app_name");
  app->window_start_cb = bind(&MyApp::OnWindowStart, app, _1);
  return app;
}

extern "C" int MyAppMain(LFApp*) {
  if (app->Create(__FILE__, "com.lucidfusionlabs.IdleShine")) return -1;
  if (app->Init()) return -1;

  app->StartNewWindow(app->focused);
  return app->Main();
}
