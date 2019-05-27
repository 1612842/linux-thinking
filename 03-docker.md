# Tất tần cơ bản về Docker

<div align="center">
    <img alt="linux" src="https://images.viblo.asia/117ac0d5-3872-4a83-b27a-487089594df2.png">
</div>

# Giới thiệu 
-   Docker có rất nhiều định nghĩa:
    -   Docker là một dự án `mã nguồn mở` giúp tự động triển khai các ứng dụng Linux và Windows vào trong các `container` ảo hóa
    -   Nói cách khác, Docker là một `open platform` cung cấp cho người sử dụng các công cụ và server để có thể `đóng gói` và `chạy chương trình` của mình trên `môi trường khác nhau` nhanh nhất có thể
    -   Hiểu đơn giản, Docker là một phương thức để đóng gói và sắp xếp phần mềm

-   Docker cung cấp khả năng đóng gói hiệu quả, ta thiết lập các cấu hình môi trường ban đầu, khi ai đó muốn chạy app họ không cần phải biết về môi trường chạy app như java, ruby on rails,...

-   Thành phần cơ bản:
    -   `Docker engine`: tạo ra Docker image và Docker container
    -   `Docker Hub`: service lưu trữ các Docker image
-   Thuật ngữ:
    -   `Docker machine`: tạo ra các Docker engine trên máy chủ
    -   `Docker compose`: chạy app bằng cách define cấu hình các Docker thông qua tệp cấu hình
    -   `Docker image`: 
        -   Tập hợp các tệp của ứng dụng, được tạo ra bởi `Docker engine`, nội dung của image không bị thay đổi khi di chuyển. 
        -   Docker image dùng để chạy các Docker container
        -   Đây là một template chỉ cho phép đọc, 1 image có thể chứa hệ điều hành ubuntu và apps
    -   `Docker container`: 
        -   Một dạng run time của các `Docker image` dùng để làm môi trường chạy ứng dụng
        -   Có nét giống với directory giữ mọi thứ chúng ta cần để chạy 1 app
        -   Mỗi container được tạo từ Docker image. Docker container có thể có các trạng thái run, started, stopped, moved và deleted

# Container

## Tiểu sử 
-   Ngày xưa, mỗi máy chỉ chạy 1 OS có cấu tạo sau: `máy chủ vật lý` + `hệ điều hành (OS)` + `application`
  
    <div align="center">
    <img alt="linux" src="https://viblo.asia/uploads/19a271ca-f444-4bd5-b04a-f1b9c5ce10fd.png">
    </div>

-   Sau đó, để có thể tận dụng phần cứng nhiều hơn, công nghệ ảo hóa `virtualization` ra đời như Virtual Box hay VMware. Công nghệ này có thể cài đặt nhiều OS nhưng lại bị hạn chế về tài nguyên và thời gian
    -   Tài nguyên: bật máy ảo lên không làm gì nhưng máy thật phải cung cấp tài nguyên do phải cung cấp cứng dung lượng ổ cứng và RAM
    -   Thời gian: khởi động khá lâu, lên tới hàng phút

    <div align="center">
    <img alt="linux" src="https://viblo.asia/uploads/f83e4a3a-bc95-4a4d-af37-dbaa9e03d28f.png">
    </div>

-   Để giải quyết vấn đề của `virtualiztion`, công nghệ `containerlization` ra đời

     <div align="center">
    <img alt="linux" src="https://viblo.asia/uploads/5fdfbb3b-de87-4b24-a65c-3cf8753bfa15.png">
    </div>

    -   Trên 1 máy chủ vật lý, sinh ra nhiều máy con, máy con này dùng chung phần nhân của máy mẹ (Host OS) và chia sẻ nhau tài nguyên máy mẹ
    -   Khi nào tài nguyên cần thì cấp, sài bao nhiêu cấp bấy nhiêu

## Đặc điểm

-   Các phần mềm sẽ được `Container Engine` đóng gói thành các `container`
-   `Container` là giải pháp chuyển giao phần mềm đáng tin cậy giữa các `môi trường` máy tính khác nhau bằng cách
    -   Tạo ra môi trường chứa mọi thứ mà phần mềm chạy được
    -   Không bị hệ thống làm ảnh hưởng
    -   Không ảnh hưởng hệ thống
-   Ví dụ: nếu bạn có một phần mềm Java, bạn không cần cài đặt JDK mà chỉ cần tìm container đã có setting tương ứng chạy nó lên, cho phần mềm đó chạy trong container đó, khi không sài thì tắt hay xóa nó đi, không ảnh hưởng tới máy thật
-   Ưu điểm:
    -   Linh động: triển khai được ở bất kì đâu
    -   Nhanh: gần như tức thì do chia sẻ Host OS
    -   Nhẹ: container sử dụng chung images không tốn nhiều disk
    -   Đồng nhất: không bị sự sai khác về mặt môi trường khi làm team
    -  Đóng gói: ẩn môi trường bao gồm cả app vào trong 1 gói được gọi là container
-   Nhược điểm:
    -   Nếu Host OS có lỗ hỏng ở kernel sẽ ảnh hưởng toàn bộ container có trong host đấy
    -   Người dùng hoặc ứng dụng nào đấy trong container chiếm được quyền root có thể lấy data hay điều khiển máy host cũng như container khác
# Cách hoạt động
-   Docker image là nền tảng của container, là khung xương để định hình container, khi chạy image sẽ tạo ra container. Giống như class so với thực thể
-   Đối với Docker cần phải có image và container:
    -   Container: tương tự như máy ảo, xuất hiện khi chạy image
    -   Image: không phải file vật lý mà chứa trong Docker. Một image bao gồm hệ điều hành (Windows, Ubuntu, …) và các môi trường lập trình được cài sẵn (httpd, mysqld, nginx, python, git, …)
-   Bên cạnh đó, Docker Hub là store lưu trữ các file images hỗ trợ set up nhanh cho người dùng

-   Docker engine là ứng dụng client-server với các component chính:
    -   1 server là loại long-running program gọi là daemon process
    -   REST API mà chương trình dùng để nói chuyện với daemon
    -   Command line interface client
-   CLI sẽ dùng REST API để điều khiển hoặc tương tác với Daemon thông script hoặc CLI commands 
<div align="center">
    <img alt="linux" src="https://docs.docker.com/engine/images/engine-components-flow.png">
</div>

-   Kiến trúc Docker

<div align="center">
    <img alt="linux" src="https://docs.docker.com/engine/images/architecture.svg">
</div>

-   Docker daemon lắng nghe Docker API request và quản lý Docker object

-   Docker registries lưu trữ các Docker images, Docker Hub là 1 loại reigistry public mà ai cũng có thể dùng 

-   Client gửi các commands tới docker daemon, docker daemon thực hiện chúng. Ví dụ nếu build thì client - docker daemon - images, nếu pull thì client - docker daemon - registry - images, nếu run thì client - docker daemon - images - containers

# Các lệnh cơ bản 
-   Cài đặt Docker:

```bash
$ sudo apt-get update
$ sudo apt-get install docker.io
```

-   Kiểm tra Docker đã cài đặt thành công

```bash
$ docker -v
Docker version 18.09.2, build 6247962
```


# Viết Dockerfile cơ bản

-   `Dockerfile` là một tập tin dạng text chứa một tập các câu lệnh để tạo mới một Image trong Docker
-   Một số lệnh trong Dockerfile:
    -   **FROM <base_image>:<phiên_bản>**: đây là câu lệnh bắt buộc phải có trong bất kỳ Dockerfile nào. Nó dùng để khai báo base Image mà chúng ta sẽ build mới Image của chúng ta
    -   **MAINTAINER <tên_tác_giả>**: câu lệnh này dùng để khai báo trên tác giả tạo ra Image, chúng ta có thể khai báo nó hoặc không
    -   **RUN <câu_lệnh>**: chúng ta sử dụng lệnh này để chạy một command cho việc cài đặt các công cụ cần thiết cho Image của chúng ta
    -   **CMD <câu_lệnh>**: trong một Dockerfile thì chúng ta chỉ có duy nhất một câu lệnh CMD, câu lệnh này dùng để xác định quyền thực thi của các câu lệnh khi chúng ta tạo mới Image
    -   **ADD \<src> \<dest>**: câu lệnh này dùng để copy một tập tin local hoặc remote nào đó (khai báo bằng \<src>) vào một vị trí nào đó trên Container (khai báo bằng \<dest>)
    -   **ENV <tên_biến>**: định nghĩa biến môi trường trong Container
    -   **ENTRYPOINT <câu_lệnh>**: định nghĩa những command mặc định, cái mà sẽ được chạy khi container running
    -   **VOLUME <tên_thư_mục>**: dùng để truy cập hoặc liên kết một thư mục nào đó trong Container
  
-   Ta bắt đầu tạo thư mục và file Docker:

```bash
$ mkdir docker-image
$ cd docker-image
$ touch Dockerfile
$ touch script.sh
```

-   Trong Dockerfile

```docker
FROM alpine

MAINTAINER conglt
 
COPY script.sh /script.sh

CMD ["/script.sh"]
```

-   Trong file script.sh:

```bash
#! bin/sh

echo hello world, from a script file!
```

-   Sau đó chạy lệnh:

```bash
$ docker build .

Sending build context to Docker daemon  3.072kB
Step 1/4 : FROM alpine
 ---> 055936d39205
Step 2/4 : MAINTAINER conglt
 ---> Using cache
 ---> d25d4207ec75
Step 3/4 : ADD script.sh /script.sh
 ---> c9f7ed873055
Step 4/4 : CMD ["./script.sh"]
 ---> Running in acd8f3eacb82
Removing intermediate container acd8f3eacb82
 ---> 4c67c946d5e5
Successfully built 4c67c946d5e5
```

-   Sau khi build thành công, chạy run image:

```bash
$ docker run --name test 4c67c946d5e5
hello world, from a script file!
```


# Thực hành với Xếp bi

-   Ta có thể thực hiện 2 cách 
    -   Cách 1: chỉ cần deploy file out đã compile từ gcc

        ```docker
        FROM gcc:7.4.0 
        MAINTAINER conglt
        RUN mkdir /home/code
        WORKDIR /home/code
        COPY ./code .
        ENTRYPOINT ["./server.o"]
        ```

    -   Cách 2: thực hiện makefile để compile mỗi lần chạy docker

        [Makefile](exercise/docker-image/code/Makefile)

        [File script run server](exercise/docker-image/code/runServerXepBi.sh)

        ```docker
        FROM gcc:7.4.0
        MAINTAINER conglt
        RUN mkdir /home/code
        WORKDIR /home/code
        COPY ./code .
        RUN apt-get update
        CMD chmod +x ./*
        ENTRYPOINT ["./runServerXepBi.sh"]
        ```

-   Build file images
  
![](media/buildimages.png)

-   Chạy server trên Docker
  
![](media/servertcp.png)

-   Test client kết nối từ máy thật vào máy ảo Docker
  
![](media/clienttcp.png)


# Nguồn tham khảo

https://docs.docker.com/engine/docker-overview/

https://docs.docker.com/install/

https://viblo.asia/p/docker-nhung-kien-thuc-co-ban-phan-1-bJzKmM1kK9N

https://viblo.asia/p/docker-chua-biet-gi-den-biet-dung-phan-1-lich-su-ByEZkWrEZQ0