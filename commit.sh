git add ./*

echo ""

read -p "Commit message: " message

git commit -m "$message"
git push origin master


