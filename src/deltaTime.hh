#pragma once

#include <chrono>
#include <functional>

/// Keep track of a rolling average
struct DeltaTime
{
  explicit DeltaTime(std::function<float()> finder);

  /// Update and evaluate the state of this timer
  void update();

  /// Execute the target finder function and get the result
  double getTarget();

  /// Set the target finder function; 
  /// this determines what the end goal of this timer is.
  /// Once the target the callback returns has been met or exceeded, deltaF is updated, accumulation is reset,
  /// and any excess time over the target is added back into the accumulation buffer.
  /// \param finder A function that returns a numerical target
  void setTargetFinder(std::function<float()> finder);

  /// Roll over the timer once the target has been reached
  void onTargetReached();

  /// Evaluates whether this timers target has been reached
  [[nodiscard]] bool isTargetReached();

  float accumulation = 0.0; //Counter to track when the next frame should be simulated
  float deltaC = 0.0; //The delta between checks to see if the next frame should be simulated
  float deltaF = 0.0; //The delta between frames
  std::chrono::steady_clock::time_point now;
  std::chrono::steady_clock::time_point prev;
  std::chrono::steady_clock::time_point prevFrame;

  private:
  std::function<float()> findTarget;
};
