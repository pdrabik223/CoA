//
// Created by piotr on 05/10/2021.
//

#include "window.h"

void Window::MainLoop() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 10;

  sf::RenderWindow window(sf::VideoMode(width_, height_), "CoA",
                          sf::Style::None, settings);

  window.setPosition(sf::Vector2i(position_.x, position_.y));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  // window.display();
  sf::Clock clock;

  while (window.isOpen()) {

    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (window.pollEvent(event_)) {
      if (event_.type == sf::Event::Closed)
        window.close();
      else if (event_.type == sf::Event::KeyPressed) {
        if (event_.key.code == sf::Keyboard::Space) {

          for (int i = 0; i < 10; i++) {
            if (GetQueueSize() < 1) break;
            PopFrame();
          }
        } else if (event_.key.code == sf::Keyboard::Escape) {
          window.close();
        }
      }
    }

    //    if(clock.getElapsedTime().asMilliseconds()<150) continue;
    //    clock.restart();

    if (GetQueueSize() != 0) {
      PopFrame().DrawToWindow(window);
      window.display();
    }
  }
}

Window::Window(int width, int height) : height_(height), width_(width), position_(0, 0) {
  screen_thread_ = new std::thread(&Window::MainLoop, this);
}
Window::Window(const Coord &position, int width, int height) : height_(height), width_(width), position_(position) {
  screen_thread_ = new std::thread(&Window::MainLoop, this);
}

WindowPlane Window::PopFrame() {
  WindowPlane new_frame(width_, height_);
  const std::lock_guard<std::mutex> kLock(event_queue_mutex_);
  new_frame = frame_queue_.front();
  frame_queue_.pop();

  return new_frame;
}

void Window::PushFrame(const WindowPlane &new_frame) {

  while (GetQueueSize() > 80) std::this_thread::sleep_for(std::chrono::milliseconds(300));

  const std::lock_guard<std::mutex> kLock(event_queue_mutex_);
  frame_queue_.push(new_frame);
}
int Window::GetQueueSize() {
  const std::lock_guard<std::mutex> kLock(event_queue_mutex_);
  return frame_queue_.size();
}
void Window::SetWindowLabel(const std::string &label) {
  current_window_title_ = label;
  update_title_ = true;
}
Window &Window::operator=(const Window &other) {
  if (this == &other) return *this;
  width_ = other.width_;
  height_ = other.height_;
  current_window_title_ = other.current_window_title_;
  update_title_ = false;
  event_ = other.event_;
  frame_queue_ = other.frame_queue_;
  return *this;
}
Window::Window(const Window &other) {
  width_ = other.width_;
  height_ = other.height_;
  current_window_title_ = other.current_window_title_;
  update_title_ = false;
  event_ = other.event_;
  frame_queue_ = other.frame_queue_;
}
