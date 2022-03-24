curl http://flaskupload.*****.cn:66086/upload -F "img=@$1" | grep wget|awk -F '&nbsp' '{print $2}'
