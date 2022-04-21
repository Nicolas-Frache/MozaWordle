const host = window.location.host; // Récupère l'url de la page
const protocol = window.location.protocol; // Récupère l'url de la page
const est_dans_dict_api_endpoint = "api/estValideMot";

const est_dans_dictionnaire = (mot) => {
    const url = `${protocol}//${host}/${est_dans_dict_api_endpoint}/${mot}`;

    // Abandon de la requête au bout de 5 secondes
    const controller = new AbortController()
    setTimeout(() => controller.abort(), 5000)

    return fetch(url, {
        method: "GET",
        headers: {
            "Content-Type": "application/json"
        },
        credentials: 'include',
        signal: controller.signal
    })
        .then(response => {
            if (response.ok) {
                return response.json();
            } else {
                return Promise.reject(new Error(error.message));
            }
        })
        .then(response => {
            return response.estValide;
        })
        .catch(error => {
            return Promise.reject(new Error(error.message));
        });
};

export default {
    est_dans_dictionnaire
}