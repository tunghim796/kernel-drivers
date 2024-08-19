sudo docker build -f docker/Dockerfile -t gcc-docker .
# sudo docker run --rm -v /lib/modules/$(uname -r)/build:/usr/src/linux-headers -v $(pwd):/usr/src/app gcc-docker
#sudo docker run -it --rm gcc-docker 
