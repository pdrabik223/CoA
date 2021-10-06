//
// Created by piotr on 05/10/2021.
//

#include "window.h"

void Window::MainLoop() {
  sf::ContextSettings settings;
  settings.antialiasingLevel = 10;

  sf::RenderWindow window(sf::VideoMode(width_, height_), "CoA",
                          sf::Style::Default, settings);

  window.setPosition(sf::Vector2i(0, 0));

  window.display();
  sf::Clock clock;

  while (window.isOpen()) {

    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (window.pollEvent(event_)) {
      if (event_.type == sf::Event::Closed)
        window.close();
      else if(event_.type == sf::Event::KeyPressed){
        if(event_.key.code == sf::Keyboard::Space){
          for(int i=0;i<10;i++)
            PopFrame();
        }
      }
    }


    if(clock.getElapsedTime().asMilliseconds()<150) continue;
    clock.restart();

    if(update_title_){
      update_title_ = false;
      window.setTitle(current_window_title_);
    }

    if(frame_queue_.empty())continue;

      PopFrame().DrawToWindow(window);
      window.display();

    }
}

Window::Window(int width, int height) : height_(height), width_(width) {}
WindowPlane Window::PopFrame() {
  WindowPlane new_frame(width_, height_);

  const std::lock_guard<std::mutex> lock(event_queue_mutex_);
  new_frame = frame_queue_.front();
  frame_queue_.pop();

  return new_frame;
}

void Window::PushFrame(const WindowPlane &new_frame) {
  const std::lock_guard<std::mutex> lock(event_queue_mutex_);
  frame_queue_.push(new_frame);
}
int Window::GetQueueSize() {
  return frame_queue_.size();
}
void Window::SetWindowLabel(const std::string &label) {
  current_window_title_ = label;
  update_title_ = true;
}
