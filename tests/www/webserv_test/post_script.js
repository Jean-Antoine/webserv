// Form 1 (application/x-www-form-urlencoded)
document.getElementById("testFormUrlEncoded").addEventListener("submit", function(event) {
	event.preventDefault();
	const form = event.target;
	const formData = new FormData(form);

	// Transformer FormData en une chaîne encodée
	const urlEncodedData = new URLSearchParams();
	for (const [key, value] of formData.entries()) {
		urlEncodedData.append(key, value);
	}

	// Envoyer la requête avec le bon Content-Type
	fetch(form.action, {
		method: "POST",
		headers: {
			"Content-Type": "application/x-www-form-urlencoded",
		},
		body: urlEncodedData.toString()
	})
	.then(response => response.text())
	.then(data => {
		document.getElementById("response1").style.display = "block";
		document.getElementById("response1").textContent = `${data}`;
	})
	.catch(error => console.error("Error:", error));
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
		document.getElementById("response2").style.display = "block";
		document.getElementById("response2").textContent = `${data}`;
	})
	.catch(error => console.error("Error:", error));
});

// Form 3 (application/json)
document.getElementById("testFormJson").addEventListener("submit", function(event) {
	event.preventDefault();
	const name = document.getElementById("name3").value;
	const email = document.getElementById("mail3").value;
	const message = document.getElementById("message3").value;

	const jsonData = {
		name: name,
		mail: email,
		message: message
	};

	fetch("/webserv_test/cgi-post-json.php", {
		method: "POST",
		headers: { "Content-Type": "application/json" },
		body: JSON.stringify(jsonData)
	})
	.then(response => response.text())
	.then(data => {
		document.getElementById("response3").style.display = "block";
		document.getElementById("response3").innerHTML = `${data}`;
	});
});

// Form 4 (upload file)
document.getElementById("fileUploadForm").addEventListener("submit", function (event) {
    event.preventDefault();

    const fileInput = document.getElementById("file2");
    const filenameInput = document.getElementById("filename1");

    if (!fileInput.files.length) {
        alert("Please select a file to upload.");
        return;
    }

    if (!filenameInput.value.trim()) {
        alert("Please enter a filename.");
        return;
    }

    const file = fileInput.files[0];
    const filename = encodeURIComponent(filenameInput.value.trim());
    const formData = new FormData();
    formData.append("file", file);

    const url = `/uploads/${filename}`;
	fetch(url, {
        method: "POST",
        body: formData,
    })
        .then(async (response) => {
            const reponseText = await response.text();
            const responseContainer = document.getElementById("response4");
            responseContainer.style.display = "block";

            if (response.status === 201) {
                responseContainer.textContent = `Success uploading file: ${decodeURIComponent(filename)}`;
            } else {
                responseContainer.textContent = `Error uploading file: ${response.statusText}`;            }
        })
        .catch((error) => {
            console.error("Error:", error);
            const responseContainer = document.getElementById("response4");
            responseContainer.style.display = "block";
            responseContainer.textContent = `An error occurred: ${error.message}`;
        });
});


// Clear file input buttons
document.body.addEventListener("click", function (event) {
    if (event.target.classList.contains("clear-file-button")) {
        const fileInputId = event.target.dataset.fileInput;
        const fileInput = document.getElementById(fileInputId);
        if (fileInput) {
            fileInput.value = "";
            console.log(`Cleared file input: ${fileInputId}`);
        }
    }
});

async function deleteResource() {
	const filename = document.getElementById('filename2').value;
	if (!filename) {
		alert("Please enter a filename");
		return;
	}

	const url = `/images/${filename}`;
		try {
		const response = await fetch(url, { method: "DELETE" });
		const responseContainer = document.getElementById("response5");
		responseContainer.style.display = "block";

		if (response.ok) { // Handles 204 or any successful status
			responseContainer.textContent = `Success deleting file: ${decodeURIComponent(filename)}`;}
		else {
			const responseText = await response.text();
			responseContainer.textContent = `Error deleting file: ${response.statusText}`;
		}
	} catch (error) {
		console.error("Error:", error);
		const responseContainer = document.getElementById("response2");
		responseContainer.style.display = "block";
		responseContainer.textContent = `An error occurred: ${error.message}`;
	}
}