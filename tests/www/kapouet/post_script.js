// Form 1 (application/x-www-form-urlencoded)
document.getElementById("testFormUrlEncoded").addEventListener("submit", function(event) {
	event.preventDefault();
	const form = event.target;
	const formData = new FormData(form);
	
	fetch(form.action, {
		method: "POST",
		body: formData
	})
	.then(response => response.text())
	.then(data => {
		document.getElementById("response1").textContent = `${data}`;
	})
	.catch(error => console.error("Error:", error));
});

// Form 2: Handle remove file action
document.getElementById("removeFileButton").addEventListener("click", function () {
	const fileInput = document.getElementById("file");
	fileInput.value = ""; // Clear the file input
	console.log("File input cleared.");
});

// Form 2 (multipart/form-data)
document.getElementById("testFormMultipart").addEventListener("submit", function(event) {
	event.preventDefault();
	const form = event.target;
	const formData = new FormData(form);

	fetch(form.action, {
		method: "POST",
		body: formData
	})
	.then(response => response.text())
	.then(data => {
		document.getElementById("response2").textContent = `${data}`;
	})
	.catch(error => console.error("Error:", error));
});



// Form 3 (application/json)
document.getElementById("submitJson").addEventListener("click", function() {
	const name = document.getElementById("name3").value;
	const email = document.getElementById("mail3").value;
	const message = document.getElementById("message3").value;

	const jsonData = {
		name: name,
		mail: email,
		message: message
	};

	fetch("/kapouet/test-post-json.php", {
		method: "POST",
		headers: { "Content-Type": "application/json" },
		body: JSON.stringify(jsonData)
	})
	.then(response => response.text())
	.then(data => {
		document.getElementById("response3").innerHTML = `${data}`;  // Utilisez innerHTML pour que les <br /> soient interprétés
	});
});
