git pull git@github.com:astianmuchui/Autonomous-Robot.git &
git status &
git add . &
git commit -m "update" &

if(git push git@github.com:astianmuchui/Autonomous-Robot.git
){
    echo "Deployed Successfully"
}
else{
    echo "Deploy Failed"
}
