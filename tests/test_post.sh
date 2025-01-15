# no content type 400
# wrong content type 415

curl -X POST -d "name=John&mail=john@example.com&message=Hello" http://localhost:9999/kapouet/test-post.php
curl -X POST -F "name=John" -F "mail=john@example.com" -F "message=Hello" -F "file=@/path/to/file" http://localhost:9999/kapouet/test-post-upload.php
curl -X POST -H "Content-Type: application/json" -d '{"name": "John", "mail": "john@example.com", "message": "Hello"}' http://localhost:9999/kapouet/test-post-json.php
# tester dans dossier sans droit d'ecriture