//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SFML_WINDOW_WINDOW_H_
#define COA_SFML_WINDOW_WINDOW_H_

#include "window_plane.h"
#include <SFML/Window/Event.hpp>

#include <mutex>
#include <queue>
#include <vector>
class Window {
 public:
  Window() = delete;
  Window(int width, int height);
  Window(const Coord &position, int width, int height);
  Window(const Window &other);
  Window &operator=(const Window &other);
  WindowPlane PopFrame();
  void PushFrame(const WindowPlane &new_frame);

  int GetQueueSize();
  ~Window() {
    screen_thread_->join();
    delete screen_thread_;
  }
  void SetWindowLabel(const std::string &label);

  /// main window loop
  void MainLoop();

 protected:
  int width_;
  int height_;

  std::string current_window_title_ = "CoA";
  bool update_title_ = false;

  std::mutex event_queue_mutex_;
  std::queue<WindowPlane> frame_queue_;
  std::thread *screen_thread_;
  int display_refresh_rate_ = 15;
  sf::Event event_;

  /// position of window on the screen
  Coord position_;
};

#endif//COA_SFML_WINDOW_WINDOW_H_
