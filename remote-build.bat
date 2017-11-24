<<<<<<< HEAD
aws ec2 start-instances --instance-ids i-0d522ff99edb890ca
aws ec2 describe-instances --instance-ids i-0d522ff99edb890ca

git add .
git commit -m "Sync"
git push

bash -c "ssh ubuntu@nvmosdev.benbenng.net 'export PATH=\"/opt/cross/bin:$PATH\" && cd ~/workspace/nvmos && git pull &&./clean.sh && ./build.sh'"
=======
aws ec2 start-instances --instance-ids i-0d522ff99edb890ca
aws ec2 describe-instances --instance-ids i-0d522ff99edb890ca

git add .
git commit -m "Sync"
git push

bash -c "ssh ubuntu@nvmosdev.benbenng.net 'export PATH=\"/opt/cross/bin:$PATH\" && cd ~/workspace/nvmos &&./clean.sh && ./build.sh'"
>>>>>>> f613b3372c865fc424fe71e2708b88c623b459aa
