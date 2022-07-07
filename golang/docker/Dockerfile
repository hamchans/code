FROM golang:latest

WORKDIR /root

ADD ./apiserver /root

CMD ["go", "run", "main.go", "172.16.0.2", "8000"]
