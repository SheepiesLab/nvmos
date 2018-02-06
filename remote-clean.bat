aws ec2 start-instances --instance-ids i-0d522ff99edb890ca
aws ec2 describe-instances --instance-ids i-0d522ff99edb890ca

"C:\Program Files\Git\cmd\git.exe" add .
"C:\Program Files\Git\cmd\git.exe" commit -m "Sync"
"C:\Program Files\Git\cmd\git.exe" push

bash -c "ssh ubuntu@nvmosdev.benbenng.net 'export PATH=\"/opt/cross/bin:$PATH\" && cd ~/workspace/nvmos && git pull && ./clean.sh'"
