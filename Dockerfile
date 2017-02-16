FROM gcc
COPY . /root/stargazer
WORKDIR /root/stargazer/projects/stargazer
RUN ./build
