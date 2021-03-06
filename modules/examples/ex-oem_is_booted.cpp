/*
 * Copyright (C) 2017 ifm syntron gmbh
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distribted on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// ex-oem_is_booted.cpp
//
// Check if the camera is completly booted.
// What is mostly useful for an on camera application.
//

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <algorithm>
#include "o3d3xx_camera.h"

bool wait_for_camera(o3d3xx::Camera::Ptr cam, int seconds)
{
    seconds = std::max(seconds,1);
    std::unordered_map<std::string, std::string> sw_version = cam->GetSWVersion();
    while(sw_version["Algorithm_Version"].empty() && (seconds > 0))
    {
        std::this_thread::sleep_for (std::chrono::seconds(1));
        seconds--;
    }
    return (seconds > 0);
}

int main(int argc, const char **argv)
{
    const auto seconds_to_wait = 20;
    o3d3xx::Logging::Init();
    o3d3xx::Camera::Ptr cam = o3d3xx::Camera::Ptr(new o3d3xx::Camera());

    std::cout << "Waiting for the camera to operate: ";
    if(wait_for_camera(cam, seconds_to_wait))
    {
        std::cout << "[done]" << std::endl;
    }
    else
    {
        std::cout << "[TIMEOUT]" << std::endl;
    }
}
