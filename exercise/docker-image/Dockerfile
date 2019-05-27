 FROM gcc:7.4.0
MAINTAINER conglt
RUN mkdir /home/code
WORKDIR /home/code
COPY ./code .
RUN apt-get update
CMD chmod +x ./*
ENTRYPOINT ["./runServerXepBi.sh"]
