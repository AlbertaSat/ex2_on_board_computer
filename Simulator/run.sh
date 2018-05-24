#!/bin/bash


docker-compose build
sudo docker run -ti --rm simulator_sim:latest
