const host = window.location.host; // Récupère l'url de la page
const est_dans_dict_api_endpoint = "api/estValideMot";

const check_status = (response) => {
    if (response.ok) {
        return new Promise.resolve(response);
    } else {
        return new Promise.reject(new Error(response.statusText));
    }
};

const est_dans_dictionnaire = (mot) => {
    const url = `${host}/${est_dans_dict_api_endpoint}/${mot}`;
    console.log(url);
    fetch(url, {
        method: "GET",
        headers: {
            "Content-Type": "application/json"
        },
        credentials: 'include'
    })
        .then(check_status)
        .then(data => {
            console.log(data);
        })
        .catch(error => console.log("erreur du réseau: " + error));
};

export default {
    est_dans_dictionnaire
}