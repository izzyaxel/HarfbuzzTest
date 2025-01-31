#include "deltaTime.hh"

DeltaTime::DeltaTime(std::function<float()> finder)
{
  this->findTarget = std::move(finder);
  this->now = std::chrono::steady_clock::now();
  this->prev = std::chrono::steady_clock::now();
  this->prevFrame = std::chrono::steady_clock::now();
}

void DeltaTime::setTargetFinder(std::function<float()> finder)
{
  this->findTarget = std::move(finder);
}

void DeltaTime::update()
{
  this->now = std::chrono::steady_clock::now();
  this->deltaC = std::chrono::duration_cast<std::chrono::duration<float>>(this->now - this->prev).count();
  if(this->deltaC > this->getTarget()) this->deltaC = this->getTarget();
  this->accumulation += this->deltaC;
  this->prev = this->now;
}

double DeltaTime::getTarget()
{
  return this->findTarget();
}

void DeltaTime::onTargetReached()
{
  this->deltaF = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - this->prevFrame).count();
  this->accumulation -= this->getTarget(); //Carry over any excess time
  this->prevFrame = this->now;
}

bool DeltaTime::isTargetReached()
{
  return this->accumulation >= this->getTarget();
}
