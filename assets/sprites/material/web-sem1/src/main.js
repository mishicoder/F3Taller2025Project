document.addEventListener('DOMContentLoaded', function () {
	const login = document.querySelector(".login");
	const register = document.querySelector(".register");
	const loginform = document.getElementById('loginForm');
	const form = document.getElementById('registroForm');
	const registred = document.querySelector(".registred")
	const registers = document.querySelector(".registers")

	registers.addEventListener("click", () => {
		register.style.display = "block"
		login.style.display = "none"
	})

	registred.addEventListener("click", () => {
		register.style.display = "none"
		login.style.display = "block"
	})

	let users = JSON.parse(localStorage.getItem("users")) || []
	form.addEventListener('submit', (event) => {
		event.preventDefault();
		const nombre = document.getElementById('nombre').value.trim();
		const email = document.getElementById('email').value.trim();
		const password = document.getElementById('password').value.trim();

		if (users.find(emai => {
			return emai.email === email;
		})) {
			alert("El correo ya se ha registrado");
			return
		}

		if (nombre === '' || email === '' || password === '') {
			alert('Por favor, completa todos los campos');
			return;
		}
		if (password.length < 6) {
			alert('La contraseña debe tener al menos 6 caracteres');
			return;
		}
		alert('Registro exitoso!');

		const user = {
			"nombre": nombre,
			"email": email,
			"password": password
		}


		users.push(user)


		localStorage.setItem("users", JSON.stringify(users))
		register.style.display = "none"
		login.style.display = "block"

		form.reset();
	});
	loginform.addEventListener("submit", (event) => {
		event.preventDefault()
		const emailLogin = document.getElementById('emailLogin').value.trim();
		const passwordLogin = document.getElementById('passwordLogin').value.trim();



		if (users.find(user => {
			return user.email === emailLogin && user.password === passwordLogin
		})) {
			alert("correcto")
			bienvenido()
		} else {
			alert("error")
		}


	})
});


function bienvenido() {
	const main = document.querySelector("main");
	main.innerHTML = ""
	main.innerHTML = `
	<h1>Bienvenido</h1>
	<p>Lorem ipsum dolor sit amet consectetur adipisicing elit. Laboriosam eos odit, expedita facilis accusamus voluptatem neque sunt iste ex! Facilis, odit! Nobis ea temporibus enim accusamus aperiam ullam expedita dolores?</p>
	`
}