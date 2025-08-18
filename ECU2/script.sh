# Makefile - Add your build rules here

sudo config-pin P9_19 can
sudo config-pin P9_20 can

# Gỡ cấu hình nếu đang bật
sudo ip link set can0 down 2>/dev/null

# Thiết lập lại với tốc độ mong muốn
sudo ip link set can0 type can bitrate 500000

# Bật giao diện
sudo ip link set can0 up

# Hiển thị trạng thái
ip -details link show can0




